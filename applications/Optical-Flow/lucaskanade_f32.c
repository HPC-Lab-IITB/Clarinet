#include <stdio.h>	 
#include <inttypes.h>
#include "img_rubik.h"
// #include "img_sphere.h"
#include "clarinet.h"
#include "riscv_counters.h"

// int X,Y;
int normalise = 0;

 void getPOI(int kernelSize, int POI[][1][2])
 {
	  int count = 0;
	  int mean = kernelSize/2;
	  int xPos = mean;
	  int yPos = mean;
	  int xStep = (X-mean)/kernelSize;
	  int yStep = (Y-mean)/kernelSize;
	 
	 for (int i=0; i<yStep; i++)
	 {
		 for (int j=0; j<xStep; j++)
		 {
			 POI[count][0][1] = xPos;
			 POI[count][0][0] = yPos;
			 xPos += kernelSize;
			 count++;
		 }
		 xPos = mean;
		 yPos += kernelSize;
	 }
 }
 
 void buildA (unsigned char img[Y][X], int centerX, int centerY, int kernelSize, float A[][2])
 {
	 int mean = kernelSize/2;
	 int count = 0;
	 unsigned char home = img[centerY][centerX];
	 float Ax, Ay;
	 
	 for (int j = -mean; j<mean + 1; j++)
	 {
		 for (int i = -mean; i<mean + 1; i++)
		 {
			 if (i == 0) Ax = 0.0;
			 else Ax = (float)(home - img[centerY + j][centerX + i])/(float)i;
			 if (j == 0) Ay = 0.0;
			 else Ay = (float)(home - img[centerY + j][centerX + i])/(float)j;
			 
			 A[count][0] = Ay;
			 A[count][1] = Ax;
			 count++;
		 }
	 }
 }
 
 void buildB (
         unsigned char imgNew[Y][X]
       , unsigned char imgOld[Y][X]
       , int centerX
       , int centerY
       , int kernelSize
       , float B[]
) {
	 int mean = kernelSize/2;
	 int count = 0;
	 
	 for (int j = -mean; j < mean + 1; j++)
	 {
		 for (int i = -mean; i < mean + 1; i++)
		 {
			 B[count] = (float)(imgNew[centerY + j][centerX + i] - imgOld[centerY + j][centerX + i]);
			 count++;
		 }
	 }
 }
 
 void calcV(float A[][2], float B[], float V[][2], int kernelSize, int i)
 {
     int Arows = kernelSize * kernelSize;
     
     /********************/
     
     uint32_t ata_cycle = read_cycle();
     float AtA_f[2][2];
     for (int row = 0; row < 2; row++) {
         for (int col = 0; col < 2; col++) {
            AtA_f[row][col] = 0.0;
            for (int k=0; k<Arows; k++) {
               AtA_f[row][col] += (A[k][row] * A[k][col]);
            }	
         }
     }
     ata_cycle = read_cycle() - ata_cycle;
                 
     uint32_t atb_cycle = read_cycle();
     float AtB[2][1];
     for (int row = 0; row < 2; row++) {
         for (int col = 0; col < 1; col++) {
            AtB[row][col] = 0.0;
            for (int k=0; k<Arows; k++) {
               // AtB[row][col] += fn_float_fma (A[k][row],B[k], AtB[row][col]);
               AtB[row][col] += (A[k][row] * B[k]);
            }	
         }
     }
     atb_cycle = read_cycle() - atb_cycle;
     
    /********************/

    float AtAInv[2][2];
    float Vpt[2][1];

    // Find inverse of AtA
    float det = (AtA_f[0][0]*AtA_f[1][1])-(AtA_f[0][1]*AtA_f[1][0]);
    uint32_t vpt_cycle = 0;
    if (det != 0.0) {					
        AtAInv[0][0] = (AtA_f[1][1]/det);
        AtAInv[0][1] = (-AtA_f[0][1]/det);
        AtAInv[1][0] = (-AtA_f[1][0]/det);
        AtAInv[1][1] = (AtA_f[0][0]/det);
        
        vpt_cycle = read_cycle();
        for (int row = 0; row < 2; row++) {
             for (int col = 0; col < 1; col++) {
                Vpt[row][col] = 0.0; 
                 for (int k=0; k<2; k++) {
                     Vpt[row][col] += (AtAInv[row][k] * AtB[k][col]);
             	}	
             }
         }
         vpt_cycle = read_cycle() - vpt_cycle;
         
         V[i][0] = (Vpt[0][0]);
         V[i][1] = (Vpt[0][1]);
    }

    else {
        V[i][0] = 0.0; V[i][1] = 0.0;
    }
    printf ("ata_cycle = %d\n", ata_cycle);
    printf ("atb_cycle = %d\n", atb_cycle);
    printf ("vpt_cycle = %d\n", vpt_cycle);
 }

	 
 int main() {
    int kernelSize = 5;
      
    int length = ((X-(kernelSize/2))/kernelSize) * ((Y-(kernelSize/2))/kernelSize);	 
    int POI[length][1][2];
    getPOI(kernelSize, POI);

    printf ("GetPOI complete.\n");
    
    float V[length][2];

    float A[kernelSize*kernelSize][2];
    float B[kernelSize*kernelSize];
    uint32_t end = 0;
    uint32_t start = 0;
    uint32_t avg_cycles = 0;
    
    printf ("Processing start. X: %d, Y: %d, image count: %d, length: %d \n", X, Y, fileCount, length);
    for (int fCount=1; fCount<fileCount; fCount++) {
       // printf ("Processing images I%d and I%d ...\n", (fCount-1), fCount);
       // Instead of moving Iold and Inew around (lots of data movement),
       // depending on the fCount iteration, use the appropriate images.
       for (int i=0; i<length; i++) {
          if (fCount == 1) {
             buildA(I1, POI[i][0][1], POI[i][0][0], kernelSize, A);          
             buildB(I1, I0, POI[i][0][1], POI[i][0][0], kernelSize, B);
             start = read_cycle();
             calcV(A, B, V, kernelSize, i);			 
             end = read_cycle();
             avg_cycles += (end - start);
          } else if (fCount == 2) {
             buildA(I2, POI[i][0][1], POI[i][0][0], kernelSize, A);          
             buildB(I2, I1, POI[i][0][1], POI[i][0][0], kernelSize, B);
             start = read_cycle();
             calcV(A, B, V, kernelSize, i);			 
             end = read_cycle();
             avg_cycles += (end - start);
          } else if (fCount == 3) {
             buildA(I3, POI[i][0][1], POI[i][0][0], kernelSize, A);          
             buildB(I3, I2, POI[i][0][1], POI[i][0][0], kernelSize, B);
             start = read_cycle();
             calcV(A, B, V, kernelSize, i);			 
             end = read_cycle();
             avg_cycles += (end - start);
          }
       }
    }
    printf ("Velocity computation: %d cycles.\n", avg_cycles);
    return 0;
 }

