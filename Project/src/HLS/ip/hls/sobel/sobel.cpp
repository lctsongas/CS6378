#include "sobel.h"
#include "common/xf_headers.hpp"

uint8_t apply_sobel(uint8_t img[SIZE][SIZE], int8_t filter[2][SIZE][SIZE]){
#pragma HLS inline
	return xf::Sobel<0,SIZE,XF_8UC1,XF_8UC1,SIZE, SIZE,XF_NPPC1, false>(img, filter[0],filter[1]);
}

void sobel(AXIS_T &input, 
		   AXIS_T &output,
		   int rows, int cols){
#pragma HLS INTERFACE axis register both port=input bundle=INPUT_STREAM
#pragma HLS INTERFACE axis register both port=output bundle=OUTPUT_STREAM
#pragma HLS INTERFACE s_axilite port=rows bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=cols bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

	uint8_t LINE_BUFFER[2][MAX_WIDTH], WINDOW_BUFFER[SIZE][SIZE];
	int8_t FILTER[2][SIZE][SIZE]={{{1,0,-1},
								   {2,0,-2},
								   {1,0,-1}},
								  {{1,2,1},
								   {0,0,0},
								   {-1,-2,-1}}};

	//input--, output--;
	for(int row=0 ; row<rows ; row++){
		for(int col=0 ; col<cols ; col++){
			#pragma HLS PIPELINE II=1

			for(int k=0 ; k<SIZE ; k++){
				#pragma HLS unroll
				WINDOW_BUFFER[k][0] = WINDOW_BUFFER[k][1];
				WINDOW_BUFFER[k][1] = WINDOW_BUFFER[k][2];
			}
			WINDOW_BUFFER[0][2] = LINE_BUFFER[0][col];
			WINDOW_BUFFER[1][2] = (LINE_BUFFER[0][col] = LINE_BUFFER[1][col]);
			WINDOW_BUFFER[2][2] = (LINE_BUFFER[1][col] = input.read().data);

			// bilateral filter
			DATA_PACK datapack;
			datapack.data = apply_sobel(WINDOW_BUFFER,FILTER);
			datapack.last = (row==rows-1 && col==cols-1);
			output.write(datapack);
		}
	}
}

