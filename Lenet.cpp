//C++ Implementation of Lenet
//Authors: Selvaraj Anandaraj

#include<stdio.h>


void sigmoid(float*** x, float*** sig_out, int input_height, int input_width, int no_of_channels)
{
	int r;
	int q;
	int p; 
	
for (r=0; r< input_height; r++){
for (q=0; q< input_width; q++){
for (p=0; p< no_of_channels; p++){

{ 
if (x[p][r][q]>0){sig_out[p][r][q]= x[p][r][q]/(1+x[p][r][q]);}
if (x[p][r][q]<0){sig_out[p][r][q]= x[p][r][q]/(1-x[p][r][q]);}
}}}}

}

//void relu (float*** array, int input_height, int input_width, int no_of_channels){
//      int i, j, k;
//
//      int iterations = input_height * input_width * no_of_channels;
//
//        for(i=0; i<input_height ; i++){
//                for(j=0; j<input_width; j++){
//                        for(k=0; k< no_of_channels; k++){
//                                if(array[k][i][j]<0) array[k][i][j]=0;
//                        }
//                }
//        }
//}

// computational aspect
// convolutional layer : underlying logic: stride(op width -1)+ kernel = image width :
void conv_layer(float*** input, float*** kernel, float*** output, int width, int height, int no_of_channels, int kernel_width, int kernel_height, int stride_width, int stride_height, int no_of_filters  , int out_height, int out_width){

int c;
int x;
int y;
int w;
int z;
int oc;
float val;

for(oc = 0; oc < no_of_filters; oc++){
   for(x=0; x<out_width; x++)
   {
        for (y=0; y<out_height; y++)
        {
        val = 0;
                for(c=0; c<no_of_channels; c++)
                {
                        for(z=0;z<kernel_width;z++)
                        {
                                for (w=0;w<kernel_height;w++)
                                {

                                        val += input[c][y*stride_height+w][x*stride_width+z]*kernel[c][w][z];

                                }
                                }
                        }

                        output[oc][x][y]= val;
                }
        }
}
}
//flattening layer
void flatten (float*** input, float* out_flat_array , int no_of_channels, int input_width, int input_height){
        int c;

        int w;

        int h;

        int a=no_of_channels*input_width*input_height;
        int i=0;

        for (c=0;c<no_of_channels;c++)
        {
                for(w=0;w<input_width;w++){
                        for(h=0;h<input_height;h++)
                        {
                                out_flat_array[i]= input[c][h][w];
                                i++;
                        }
                }
        }
}
// fully connected layer
void fully_connected(float* out_fc, float* out_flat_array, float* array_weights, int no_of_weights, int total){

int k;
int j;

for(j=0; j<no_of_weights;j++){
       for(k=0; k<total; k++) {

        out_fc[j]= out_fc[j]+out_flat_array[k]*array_weights[j];}
}

}
void maxpool(float*** out, float*** in,  int no_of_channels, int height, int width, int pool_y, int pool_x, int stride_y, int stride_x)
// stride and window size
{
        int c;


        int out_height;
        int out_width;

        int x,y;
        int i;

        int j;

        float value;


for (c=0; c<no_of_channels; c++){

for (y = 0; y < out_height; ++y) {
    for (x = 0; x < out_width; ++x) {
    	out[c][y][x] = in[c][y * stride_y][x * stride_x];
        for (i = 0; i < pool_y; ++i) {
            for (j = 0; j < pool_x; ++j) {

                    value = in[c][y * stride_y + i][x * stride_x + j];
                    out[c][y][x] =out[c][x][y]>value? out[c][x][y]: value;


                        }
                        }
                }
                        }
                }
        }

void avgpool(float*** out, float*** in,  int no_of_channels, int height, int width, int pool_y, int pool_x, int stride_y, int stride_x)
// stride and window size
{
        int c;


        int out_height;
        int out_width;

        int x,y;
        int i;

        int j;



float val = 0;
for (c=0; c<no_of_channels; c++){

for (y = 0; y < out_height; ++y) {
    for (x = 0; x < out_width; ++x) {
    	val = 0;
        for (i = 0; i < pool_y; ++i) {
            for (j = 0; j < pool_x; ++j) {

                    val += in[c][y * stride_y + i][x * stride_x + j];
                        }
                        }
                        out[c][y][x] = val/(pool_x*pool_y);
                }
                        }
                }
        }
int main(){


int kernel_height, kernel_width, kernel_count;
int output_height, output_width;
int stride_x, stride_y;

int input_height = 32;
int input_width = 32;
int input_channels = 1;

kernel_height = 5;
kernel_width = 5;
kernel_count = 6;
stride_x=1;
stride_y=1;

output_width = ((input_width-kernel_width)/stride_x)+1;
output_height = ((input_height-kernel_height)/stride_y)+1;

float ***input;
float ***kernel;
float ***output;

output = new float**[kernel_count];


for (int i=0; i<kernel_count; i++){
   output[i] = new float*[output_height];
   for(int j=0; j<output_height; j++){
      output[i][j] = new float[output_width];
   }
}

input = new float**[input_channels];

for (int i=0; i<input_channels; i++){
   input[i] = new float*[input_height];
   for(int j=0; j<input_height; j++){
      input[i][j] = new float[input_width];
   }
}

kernel = new float**[kernel_count];
for(int i=0; i<kernel_count; i++){
   kernel[i]= new float*[kernel_height];
      for(int j=0; j<kernel_height; j++){
  kernel[i][j]= new float [kernel_width];
      }

}

//Alexnet Conv1
conv_layer(input,kernel,output,input_height,input_width,input_channels,kernel_height,kernel_width,stride_x,stride_y,kernel_count,output_height,output_width);

for(int i=0; i<kernel_count; i++){
      for(int j=0; j<kernel_height; j++){
  delete kernel[i][j];
      }
      delete kernel[i];
}
delete kernel;

for (int i=0; i<input_channels; i++){
   for(int j=0; j<input_height; j++){
      delete input[i][j];
   }
   delete input[i];
}
delete input;

for (int i=0; i<kernel_count; i++){
   for(int j=0; j<output_height; j++){
      delete output[i][j];
   }
   delete output[i];
}
delete output;

//sigmoid 
input_height=28;
input_width=28;
int no_of_channels=6;

float ***sig_out;
float ***x;
x = new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   x[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  x[i][j]= new float [input_width];
      }

}
sig_out= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   sig_out[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  sig_out[i][j]= new float [input_width];
      }

}

sigmoid(x, sig_out, input_height, input_width, no_of_channels);

for(int i=0; i<no_of_channels; i++){
   
      for(int j=0; j<input_height; j++){
  delete x[i][j];
      }delete x[i];
}
delete x;

for(int i=0; i<no_of_channels; i++){
   
      for(int j=0; j<input_height; j++){
  delete sig_out[i][j];
      }delete sig_out[i];
}
delete sig_out;

input_height =28;
input_width = 28;
input_channels =6;

kernel_height =2;
kernel_width =2;
stride_x=2;
stride_y=2;

output_width = ((input_width-kernel_width)/stride_x)+1;
output_height = ((input_height-kernel_height)/stride_y)+1;



float ***out;
out = new float**[input_channels];

for(int i=0; i<input_channels; i++){
   out[i]= new float*[output_height];
      for(int j=0; j<output_height; j++){
  out[i][j]= new float [output_width];
      }

}

float ***in;
in = new float**[input_channels];

for (int i=0; i<input_channels; i++){
   in[i] = new float*[input_height];
   for(int j=0; j<input_height; j++){
      in[i][j] = new float[input_width];
   }
}
//maxpooling or avg pooling 1
avgpool(out, in, input_channels, output_height , output_width, kernel_height, kernel_width,  stride_y,  stride_x);

for(int i=0; i<input_channels; i++){
   
      for(int j=0; j<output_height; j++){
  delete out[i][j];
      }delete out[i];
}
delete out;

for (int i=0; i<input_channels; i++){
   for(int j=0; j<input_height; j++){
      delete in[i][j];
   }delete in[i];
}
delete in;

//conv2 
input_height = 14;
input_width = 14;
input_channels = 6;

kernel_height =5;
kernel_width = 5;
kernel_count = 16;
stride_x=1;
stride_y=1;

output_width = ((input_width-kernel_width)/stride_x)+1;
output_height = ((input_height-kernel_height)/stride_y)+1;




output = new float**[kernel_count];


for (int i=0; i<kernel_count; i++){
   output[i] = new float*[output_height];
   for(int j=0; j<output_height; j++){
      output[i][j] = new float[output_width];
   }
}

input = new float**[input_channels];

for (int i=0; i<input_channels; i++){
   input[i] = new float*[input_height];
   for(int j=0; j<input_height; j++){
      input[i][j] = new float[input_width];
   }
}

kernel = new float**[kernel_count];
for(int i=0; i<kernel_count; i++){
   kernel[i]= new float*[kernel_height];
      for(int j=0; j<kernel_height; j++){
  kernel[i][j]= new float [kernel_width];
      }

}

conv_layer(input,kernel,output,input_height,input_width,input_channels,kernel_height,kernel_width,stride_x,stride_y,kernel_count,output_height,output_width);

for(int i=0; i<kernel_count; i++){
      for(int j=0; j<kernel_height; j++){
  delete kernel[i][j];
      }
      delete kernel[i];
}
delete kernel;

for (int i=0; i<input_channels; i++){
   
   for(int j=0; j<input_height; j++){
      delete input[i][j];
   }delete input[i];
}
delete input;

for (int i=0; i<kernel_count; i++){
   
   for(int j=0; j<output_height; j++){
      delete output[i][j];
   }delete output[i];
}
delete output;

//sigmoid
input_height = 10;
input_width = 10;
input_channels =16;

x= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   x[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  x[i][j]= new float [input_width];
      }

}
sig_out= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   sig_out[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  sig_out[i][j]= new float [input_width];
      }

}

sigmoid(x, sig_out, input_height, input_width, no_of_channels);

for(int i=0; i<no_of_channels; i++){
      for(int j=0; j<input_height; j++){
  delete x[i][j];
      }delete x[i];
}
delete x;

for(int i=0; i<no_of_channels; i++){
      for(int j=0; j<input_height; j++){
  delete sig_out[i][j];
      }delete sig_out[i];
}
delete sig_out;

//maxpool 2 
input_height =10;
input_width = 10;
input_channels =16;

kernel_height =2;
kernel_width =2;
stride_x=2;
stride_y=2;

output_width = ((input_width-kernel_width)/stride_x)+1;
output_height = ((input_height-kernel_height)/stride_y)+1;


out = new float**[input_channels];

for(int i=0; i<input_channels; i++){
   out[i]= new float*[output_height];
      for(int j=0; j<output_height; j++){
  out[i][j]= new float [output_width];
      }

}

in = new float**[input_channels];

for (int i=0; i<input_channels; i++){
   in[i] = new float*[input_height];
   for(int j=0; j<input_height; j++){
      in[i][j] = new float[input_width];
   }
}

avgpool(out, in, input_channels, output_height , output_width, kernel_height, kernel_width,  stride_y,  stride_x);

for(int i=0; i<input_channels; i++){
   
      for(int j=0; j<output_height; j++){
  delete out[i][j];
      }delete out[i];
}
delete out;

for (int i=0; i<input_channels; i++){
   
   for(int j=0; j<input_height; j++){
      delete in[i][j];
   }delete in[i];
}
delete in;

//conv3 
input_height = 5;
input_width = 5;
input_channels = 16;

kernel_height =5;
kernel_width = 5;
kernel_count = 120;
stride_x=1;
stride_y=1;

output_width = ((input_width-kernel_width)/stride_x)+1;
output_height = ((input_height-kernel_height)/stride_y)+1;

output = new float**[kernel_count];


for (int i=0; i<kernel_count; i++){
   output[i] = new float*[output_height];
   for(int j=0; j<output_height; j++){
      output[i][j] = new float[output_width];
   }
}

input = new float**[input_channels];

for (int i=0; i<input_channels; i++){
   input[i] = new float*[input_height];
   for(int j=0; j<input_height; j++){
      input[i][j] = new float[input_width];
   }
}

kernel = new float**[kernel_count];
for(int i=0; i<kernel_count; i++){
   kernel[i]= new float*[kernel_height];
      for(int j=0; j<kernel_height; j++){
  kernel[i][j]= new float [kernel_width];
      }

}

conv_layer(input,kernel,output,input_height,input_width,input_channels,kernel_height,kernel_width,stride_x,stride_y,kernel_count,output_height,output_width);

for(int i=0; i<kernel_count; i++){
      for(int j=0; j<kernel_height; j++){
  delete kernel[i][j];
      }
      delete kernel[i];
}
delete kernel;

for (int i=0; i<input_channels; i++){
   
   for(int j=0; j<input_height; j++){
      delete input[i][j];
   }delete input[i];
}
delete input;

for (int i=0; i<kernel_count; i++){
   
   for(int j=0; j<output_height; j++){
      delete output[i][j];
   }delete output[i];
}
delete output;

//sigmoid

input_height = 1;
input_width = 1;
input_channels =120;

x= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   x[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  x[i][j]= new float [input_width];
      }

}
sig_out= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   sig_out[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  sig_out[i][j]= new float [input_width];
      }

}

sigmoid(x, sig_out, input_height, input_width, no_of_channels);

for(int i=0; i<no_of_channels; i++){
   
      for(int j=0; j<input_height; j++){
  delete x[i][j];
      }delete x[i];
}
delete x;

for(int i=0; i<no_of_channels; i++){
   
      for(int j=0; j<input_height; j++){
  delete sig_out[i][j];
      }delete sig_out[i];
}
delete sig_out;

//fully connected 1 


input_height =1;
input_width = 1;
input_channels =120;

int total;

total= input_height*input_width*input_channels;
int no_of_weights= 84;

float *out_flat_array;
out_flat_array = new float[total];

float *out_fc;
out_fc = new float[no_of_weights];

float *array_weights;
array_weights = new float[no_of_weights];

fully_connected(out_fc, out_flat_array, array_weights, no_of_weights, total);

delete out_flat_array;
delete out_fc;
delete array_weights;

// sigmoid
input_height = 1;
input_width = 1;
input_channels =84;

x= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   x[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  x[i][j]= new float [input_width];
      }

}
sig_out= new float**[no_of_channels];

for(int i=0; i<no_of_channels; i++){
   sig_out[i]= new float*[input_height];
      for(int j=0; j<input_height; j++){
  sig_out[i][j]= new float [input_width];
      }

}

sigmoid(x, sig_out, input_height, input_width, no_of_channels);

for(int i=0; i<no_of_channels; i++){
   
      for(int j=0; j<input_height; j++){
  delete x[i][j];
      }delete x[i];
}
delete x;

for(int i=0; i<no_of_channels; i++){
   
      for(int j=0; j<input_height; j++){
  delete sig_out[i][j];
      }delete sig_out[i];
}
delete sig_out;

//fully connected 2


input_height =1;
input_width = 1;
input_channels =84;

total= input_height*input_width*input_channels;

no_of_weights= 10 ;
out_flat_array= new float[total];

out_fc = new float[no_of_weights];

array_weights= new float[no_of_weights];
 
fully_connected( out_fc, out_flat_array, array_weights, no_of_weights, total);

delete out_flat_array;
delete out_fc;
delete array_weights;
}

