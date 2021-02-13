// Copyright (c) HPC Lab, Department of Electrical Engineering, IIT Bombay
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
package Posit_Numeric_Types;

// ================================================================
// Basic sizes, from which everything else is derived

// PositWidth        =  8    (= 0x08)    (bits in posit number)
// ExpWidth          =  0    (= 0x00)    (width of exponent field)
// FloatWidth          = 32    (= 0x20)    (bits of Floating point number)

// ================================================================
// Type decls

// Posit Fields ---------------

typedef         8   PositWidth                    ;    // (basic)
typedef         0   ExpWidth                      ;    // (basic)
typedef        32   FloatWidth                    ;    // (basic)
typedef         8   FloatExpWidth                 ;    // Exponent width of single precision float
typedef        23   FloatFracWidth                ;    // Fraction width of single precision float
typedef       127   FloatBias                     ;    // Bais for single precision float
typedef         7   PositWidthMinus1              ;    // PositWidth - 1
typedef         5   PositWidthMinus3              ;    // PositWidth - 3
typedef         3   BitsPerPositWidth             ;    // log2 (PositWidth)
typedef         3   Iteration                     ;    // log2 (PositWidth-1)
typedef         4   RegimeWidth                   ;    // log2 (PositWidth) + 1

typedef         1   MaxExpValue                   ;    // 2 ^ ExpWidth
typedef         0   BitsPerExpWidth               ;    // log2 (ExpWidth)

typedef         5   FracWidth                     ;    // PositWidth-3-ExpWidth

typedef         3   ScaleWidth                    ;    // log2((PositWidth-1)*(2^ExpWidth)-1)
typedef         4   ScaleWidthPlus1               ;    // ScaleWidth + 1
typedef         4   FracWidthMinus1               ;    // FracWidth - 1
typedef         3   ScaleWidthMinusExpWidth       ;    // ScaleWidth - ExpWidth
typedef         4   ScaleWidthMinusExpWidthPlus1  ;    // ScaleWidth - ExpWidth
typedef        22   FloatFracWidthMinus1          ;    // (FloatFracWidth-1)
typedef        18   FloatFracWidthMinusFracWidth  ;    // (FloatFracWidth-FracWidth)
typedef         4   LogFracWidthPlus1             ;    // log2 (FracWidth) + 1
typedef         6   LogFloatFracWidthPlus1        ;    // log2 (FloatFracWidth) + 1
typedef        20   FracWidthMul4                 ;    // FracWidth * 4
typedef        21   FracWidthMul4Plus1            ;    // FracWidthMul4 + 1
typedef         5   LogFracWidthMul4              ;    // log2 (FracWidthMul4)
typedef         6   LogFracWidthMul4Plus1         ;    // LogFracWidthMul4 + 1


/*// ================================================================
// Utility functions

function  Bit#(1)  fnSign (Bit #(n)  posit);
   return  posit [(valueOf (n) - 1)];
endfunction

function  Bit#(n)  fnUnsignedPosit (Bit #(n)  posit);
   return  extend (posit [(valueOf (n) - 1):0]);
endfunction
// ================================================================*/
//function defines shift in fraction depending on number of fraction bit change from float to posit
function Tuple3#(Bit#(FracWidth), Bit#(1), Bit#(1)) fv_calculate_frac_fp(Bit#(FloatFracWidth) frac);
begin 
	let a_frac_truncate = valueOf(FloatFracWidthMinusFracWidth);
	Bit#(1) truncated_frac_msb = a_frac_truncate > 0 ? frac[a_frac_truncate-1]:1'b0;
	Bit#(TSub#(FloatFracWidthMinusFracWidth,1)) frac_truncate_zero = truncate(frac);
	Bit#(1) truncated_frac_zero = a_frac_truncate > 1 ? pack(frac_truncate_zero ==  0):1'b1;
	return tuple3(frac[valueOf(FloatFracWidthMinus1):a_frac_truncate],truncated_frac_msb,truncated_frac_zero);
end 
endfunction
//function defines shift in fraction depending on number of fraction bit change from posit to float
 function Tuple3#(Bit#(FloatFracWidth), Bit#(1), Bit#(1)) fv_calculate_frac_pf(Bit#(FracWidth) frac);
begin 
	Bit#(FracWidth) frac_extend= frac[valueOf(FracWidthMinus1):0]; 
	return tuple3({frac_extend,'0},1'b0,1'b1); 
end 
endfunction

//This function checks if the scale value has exceeded the limits max and min set due to the restricted availability of regime bits 
// fraction bits will be shifted to take care of the scale value change due to it being bounded 
//output : bounded scale value and the shift in frac bits 
//the shift in frac bits is to be bounded becoz of the change in bit sizes 
function Tuple2#(Int#(ScaleWidthPlus1), Int#(LogFracWidthPlus1)) fv_calculate_scale_shift_fp(Int#(FloatExpWidth) scale);
	Int#(ScaleWidthPlus1) maxB,minB,scale1; 
	Int#(FloatExpWidth) frac_change; 
	Int#(LogFracWidthPlus1) frac_change_bounded; 
	//max scale value is defined here... have to saturate the scale value 
	// max value = (N-2)*(2^es)  
	// scale = regime*(2^es) + expo.... max value of regime = N-2(00...1) 
	maxB = fromInteger((valueOf(PositWidth) -2)*(2**(valueOf(ExpWidth)))); 
	//similarly calculate the min 
	minB = -maxB; 
	//frac_change gives the number of bits that are more or less than scale bounds so that we can shift the frac bits to not lose scale information 
	Int#(LogFracWidthPlus1) max_frac = unpack({1'b0,'1}); 
	Int#(LogFracWidthPlus1) min_frac = unpack({1'b1,extend(1'b1)}); 

	Int#(FloatExpWidth) max_frac_extend = signExtend(max_frac); 
	Int#(FloatExpWidth) min_frac_extend = signExtend(min_frac); 

	if (scale<extend(minB)) 
	begin 
		frac_change = truncate(scale - extend(minB));// find the change in scale to bind it 
		scale1 = minB;//bound scale 
		frac_change_bounded = truncate(max(frac_change,min_frac_extend));
end 
	else if (scale>extend(maxB)) 
	begin 
		frac_change = truncate(scale - extend(maxB));// find the change in scale to bind it 
		scale1 = maxB;//bound scale 
		frac_change_bounded = truncate(min(frac_change,max_frac_extend));
end 
	else 
	begin 
		frac_change_bounded = fromInteger(0); 
		scale1 = truncate(scale);//no change 
	end 
	return tuple2(scale1,frac_change_bounded); 
endfunction 

//This function checks if the scale value has exceeded the limits max and min set due to the restricted availability of regime bits 
// fraction bits will be shifted to take care of the scale value change due to it being bounded 
//output : bounded scale value and the shift in frac bits 
//the shift in frac bits is to be bounded becoz of the change in bit sizes 
function Tuple2#(Int#(FloatExpWidth), Int#(LogFloatFracWidthPlus1)) fv_calculate_scale_shift_pf(Int#(ScaleWidthPlus1) scale);
	Int#(FloatExpWidth) maxB,minB,scale1; 
	Int#(ScaleWidthPlus1) frac_change; 
	Int#(LogFloatFracWidthPlus1) frac_change_bounded; 
	//max scale value is defined here... have to saturate the scale value 
	// max value = (N-2)*(2^es)  
	// scale = regime*(2^es) + expo.... max value of regime = N-2(00...1) 
	maxB = fromInteger(valueOf(FloatBias)); 
	//similarly calculate the min 
	minB = -fromInteger(valueOf(FloatBias)); 
	//frac_change gives the number of bits that are more or less than scale bounds so that we can shift the frac bits to not lose scale information 
	Int#(LogFloatFracWidthPlus1) max_frac = unpack({1'b0,'1}); 
	Int#(LogFloatFracWidthPlus1) min_frac = unpack({1'b1,extend(1'b1)}); 

	Int#(ScaleWidthPlus1) max_frac_extend = truncate(max_frac); 
	Int#(ScaleWidthPlus1) min_frac_extend = truncate(min_frac); 

	if (extend(scale)<(minB)) 
	begin 
		frac_change = truncate(extend(scale) - (minB));// find the change in scale to bind it 
		scale1 = minB;//bound scale 
		frac_change_bounded = extend(max(frac_change,min_frac_extend));
	end 
	else if (extend(scale)>(maxB)) 
	begin 
		frac_change = truncate(extend(scale) - (maxB));// find the change in scale to bind it 
		scale1 = maxB;//bound scale 
		frac_change_bounded = extend(min(frac_change,max_frac_extend));
	end 
	else 
	begin 
		frac_change_bounded = fromInteger(0); 
		scale1 = extend(scale);//no change 
	end 
	return tuple2(scale1,frac_change_bounded); 
endfunction 
function Bit#(ScaleWidthPlus1) fv_frac_shit_adder_mac(Bit#(FracWidthMul4Plus1) frac_sum); 
	Bit#(LogFracWidthMul4Plus1) frac_shift_1; 
	Bit#(ScaleWidthPlus1) zero_one = '1; 
	Bit#(ScaleWidthPlus1) frac_shift; 
	frac_shift_1 = min(extend(zero_one),extend(pack(countZerosMSB(frac_sum)))); 
	frac_shift =  truncate(frac_shift_1);
	return frac_shift; 
endfunction 

// ================================================================

endpackage: Posit_Numeric_Types
