#include "conv.hpp"

void Conv2d::forward(const float* input, float* output)
{
    // convert 1D arrays to 4D arrays
    float (*weights_4d)[out_channels][kernel_size][kernel_size] = (float (*)[out_channels][kernel_size][kernel_size])weights;

    float (*input_4d)[in_channels][input_size[2]][input_size[3]] = (float (*)[in_channels][input_size[2]][input_size[3]])input;
    float (*output_4d)[out_channels][output_size[2]][output_size[3]] = (float (*)[out_channels][output_size[2]][output_size[3]])output;

    for (int b = 0; b < input_size[0]; b++)
    {
        for (int oc = 0; oc < out_channels; oc++)
        {
            for (int oh = 0; oh < output_size[2]; oh++)
            {
                for (int ow = 0; ow < output_size[3]; ow++)
                {
                    for (int ih = 0; ih < input_size[2]; ih++)
                    {
                        for (int iw = 0; iw < input_size[3]; iw++)
                        {
                            float sum = 0.0f;
                            for (int ic = 0; ic < in_channels; ic++)
                            {
                                for (int kh = 0; kh < kernel_size; kh++)
                                {
                                    for (int kw = 0; kw < kernel_size; kw++)
                                    {
                                        int ih_padded = ih - padding + kh * stride;
                                        int iw_padded = iw - padding + kw * stride;
                                        if (ih_padded >= 0 && ih_padded < input_size[2] && iw_padded >= 0 && iw_padded < input_size[3])
                                        {
                                            sum += input_4d[b][ic][ih][iw] * weights_4d[oc][ic][kh][kw];
                                        }
                                    }
                                }
                                output_4d[b][oc][oh][ow] = sum + bias[oc];
                            }
                        }
                    }
                }
            }
        }
    }
}