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

// PositWidth        = 32    (= 0x20)    (bits in posit number)
// ExpWidth          =  2    (= 0x02)    (width of exponent field)
// FloatWidth          = 32    (= 0x20)    (bits of Floating point number)

// ================================================================
// Type decls

// Posit Fields ---------------

typedef        32   PositWidth                    ;    // (basic)
typedef         2   ExpWidth                      ;    // (basic)
typedef        32   FloatWidth                    ;    // (basic)
typedef         8   FloatExpWidth                 ;    // Exponent width of single precision float
typedef        23   FloatFracWidth                ;    // Fraction width of single precision float
typedef       127   FloatBias                     ;    // Bais for single precision float
typedef        31   PositWidthMinus1              ;    // PositWidth - 1
typedef        29   PositWidthMinus3              ;    // PositWidth - 3
typedef         5   BitsPerPositWidth             ;    // log2 (PositWidth)
typedef         5   Iteration                     ;    // log2 (PositWidth-1)
typedef         6   RegimeWidth                   ;    // log2 (PositWidth) + 1

typedef         4   MaxExpValue                   ;    // 2 ^ ExpWidth
typedef         1   BitsPerExpWidth               ;    // log2 (ExpWidth)

typedef        27   FracWidth                     ;    // PositWidth-3-ExpWidth

typedef         7   ScaleWidth                    ;    // log2((PositWidth-1)*(2^ExpWidth)-1)
typedef         8   ScaleWidthPlus1               ;    // ScaleWidth + 1
typedef        26   FracWidthMinus1               ;    // FracWidth - 1
typedef         5   ScaleWidthMinusExpWidth       ;    // ScaleWidth - ExpWidth
typedef         6   ScaleWidthMinusExpWidthPlus1  ;    // ScaleWidth - ExpWidth
typedef        22   FloatFracWidthMinus1          ;    // (FloatFracWidth-1)
typedef         4   FracWidthMinusFloatFracWidth  ;    // (FracWidth-FloatFracWidth)
typedef         6   LogFracWidthPlus1             ;    // log2 (FracWidth) + 1
typedef         6   LogFloatFracWidthPlus1        ;    // log2 (FloatFracWidth) + 1
typedef       108   FracWidthMul4                 ;    // FracWidth * 4
typedef       109   FracWidthMul4Plus1            ;    // FracWidthMul4 + 1
typedef         7   LogFracWidthMul4              ;    // log2 (FracWidthMul4)
typedef         8   LogFracWidthMul4Plus1         ;    // LogFracWidthMul4 + 1


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
	Bit#(FracWidth) frac_extend={frac,'0}; 
	return tuple3(frac_extend,1'b0,1'b1); 
end 
endfunction
//function defines shift in fraction depending on number of fraction bit change from posit to float
 function Tuple3#(Bit#(FloatFracWidth), Bit#(1), Bit#(1)) fv_calculate_frac_pf(Bit#(FracWidth) frac);
begin 
	let a_frac_truncate = valueOf(FracWidthMinusFloatFracWidth);
	Bit#(1) truncated_frac_msb = a_frac_truncate > 0 ? frac[a_frac_truncate-1]:1'b0;
	Bit#(TSub#(FracWidthMinusFloatFracWidth,1)) frac_truncate_zero = truncate(frac);
	Bit#(1) truncated_frac_zero = a_frac_truncate > 1 ? pack(frac_truncate_zero ==  0):1'b1;
	return tuple3(frac[valueOf(FracWidthMinus1):a_frac_truncate],truncated_frac_msb,truncated_frac_zero);
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

	if (extend(scale)<(minB)) 
	begin 
		frac_change = truncate(extend(scale) - (minB));// find the change in scale to bind it 
		scale1 = minB;//bound scale 
		frac_change_bounded = truncate(max(frac_change,min_frac_extend));
end 
	else if (extend(scale)>(maxB)) 
	begin 
		frac_change = truncate(extend(scale) - (maxB));// find the change in scale to bind it 
		scale1 = maxB;//bound scale 
		frac_change_bounded = truncate(min(frac_change,max_frac_extend));
end 
	else 
	begin 
		frac_change_bounded = fromInteger(0); 
		scale1 = extend(scale);//no change 
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

	Int#(ScaleWidthPlus1) max_frac_extend = signExtend(max_frac); 
	Int#(ScaleWidthPlus1) min_frac_extend = signExtend(min_frac); 

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
function Bit#(ScaleWidthPlus1) fv_frac_shit_adder_mac(Bit#(FracWidthMul4Plus1) frac_sum); 
	Bit#(LogFracWidthMul4Plus1) frac_shift_1; 
	Bit#(ScaleWidthPlus1) zero_one = '1; 
	Bit#(ScaleWidthPlus1) frac_shift; 
	frac_shift_1 = min(truncate(zero_one),extend(pack(countZerosMSB(frac_sum)))); 
	frac_shift =  extend(frac_shift_1);
	return frac_shift; 
endfunction 

// ================================================================

endpackage: Posit_Numeric_Types
