#include "ViewWindow.h"

#include "TRootEmbeddedCanvas.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TColor.h"

#include <iostream>
using namespace std;


ViewWindow::ViewWindow(const TGWindow *p, int w,int h)
    :TRootEmbeddedCanvas("ECanvas", p, w, h)
{
    can = GetCanvas();
    InitCanvas();
    InitStyle();
}

ViewWindow::~ViewWindow()
{
}


void ViewWindow::InitCanvas()
{
    can->cd();
    can->Divide(1, 3, 0.005, 0.005);
    SetTheme(0);
}

void ViewWindow::SetTheme(int theme)
{
    for (int i=1; i<=3; i++) {
        can->cd(i);
        if (theme == 0) {
            // gPad->SetFillColor(TColor::GetColor(222,186,135));
            gPad->SetFillColor(TColor::GetColor(210,210,210));
        }
        else {
            gPad->SetFillColor(kWhite);
        }
        gPad->SetGridx();
        gPad->SetGridy();
    }
}

void ViewWindow::InitStyle()
{
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    gStyle->SetTitleStyle(0);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleOffset(1.0, "x");
    gStyle->SetTitleOffset(1.0, "y");
    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);
    gStyle->SetLabelSize(0.06, "x");
    gStyle->SetLabelSize(0.06, "y");
    gStyle->SetHistLineWidth(2);
    gStyle->SetLegendBorderSize(0);
    
    gStyle->SetPadLeftMargin(0.08);
    gStyle->SetPadRightMargin(0.04);
    gStyle->SetMarkerSize(0.3);


    PaletteRainbow();

    gROOT->ForceStyle();
}


void ViewWindow::PaletteRainbow()
{
    gStyle->SetFrameFillColor(TColor::GetColor(float(0.1), float(0.1), float(0.1)));

    // http://diana.parno.net/thoughts/?p=28
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

}

void ViewWindow::PaletteGray()
{
    // gray scale, night
    gStyle->SetFrameFillColor(TColor::GetColor(float(0.1), float(0.1), float(0.1)));
    const UInt_t Number = 2;
    Int_t nb=50;
    Double_t Red[Number]   = { 0.15, 1.00};
    Double_t Green[Number] = { 0.15, 1.00};
    Double_t Blue[Number]  = { 0.15, 1.00};
    Double_t Stops[Number] = { 0, 1.00};
    TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,nb);
    gStyle->SetNumberContours(nb);
}

void ViewWindow::PaletteSummer()
{
    gStyle->SetFrameFillColor(kWhite);
    const Int_t NRGBs = 256;
    const Int_t NCont = 256;
    Double_t stops[NRGBs] = {0.000,0.004,0.008,0.012,0.016,0.020,0.024,0.027,0.031,0.035,0.039,0.043,0.047,0.051,0.055,0.059,0.063,0.067,0.071,0.075,0.078,0.082,0.086,0.090,0.094,0.098,0.102,0.106,0.110,0.114,0.118,0.122,0.125,0.129,0.133,0.137,0.141,0.145,0.149,0.153,0.157,0.161,0.165,0.169,0.173,0.176,0.180,0.184,0.188,0.192,0.196,0.200,0.204,0.208,0.212,0.216,0.220,0.224,0.227,0.231,0.235,0.239,0.243,0.247,0.251,0.255,0.259,0.263,0.267,0.271,0.275,0.278,0.282,0.286,0.290,0.294,0.298,0.302,0.306,0.310,0.314,0.318,0.322,0.325,0.329,0.333,0.337,0.341,0.345,0.349,0.353,0.357,0.361,0.365,0.369,0.373,0.376,0.380,0.384,0.388,0.392,0.396,0.400,0.404,0.408,0.412,0.416,0.420,0.424,0.427,0.431,0.435,0.439,0.443,0.447,0.451,0.455,0.459,0.463,0.467,0.471,0.475,0.478,0.482,0.486,0.490,0.494,0.498,0.502,0.506,0.510,0.514,0.518,0.522,0.525,0.529,0.533,0.537,0.541,0.545,0.549,0.553,0.557,0.561,0.565,0.569,0.573,0.576,0.580,0.584,0.588,0.592,0.596,0.600,0.604,0.608,0.612,0.616,0.620,0.624,0.627,0.631,0.635,0.639,0.643,0.647,0.651,0.655,0.659,0.663,0.667,0.671,0.675,0.678,0.682,0.686,0.690,0.694,0.698,0.702,0.706,0.710,0.714,0.718,0.722,0.725,0.729,0.733,0.737,0.741,0.745,0.749,0.753,0.757,0.761,0.765,0.769,0.773,0.776,0.780,0.784,0.788,0.792,0.796,0.800,0.804,0.808,0.812,0.816,0.820,0.824,0.827,0.831,0.835,0.839,0.843,0.847,0.851,0.855,0.859,0.863,0.867,0.871,0.875,0.878,0.882,0.886,0.890,0.894,0.898,0.902,0.906,0.910,0.914,0.918,0.922,0.925,0.929,0.933,0.937,0.941,0.945,0.949,0.953,0.957,0.961,0.965,0.969,0.973,0.976,0.980,0.984,0.988,0.992,0.996,1.000,};

    // Double_t red[NRGBs]   = {1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.998,0.998,0.998,0.998,0.998,0.998,0.998,0.998,0.997,0.997,0.997,0.997,0.997,0.996,0.996,0.996,0.996,0.995,0.995,0.995,0.994,0.994,0.994,0.993,0.993,0.992,0.992,0.992,0.991,0.990,0.990,0.989,0.989,0.988,0.987,0.987,0.986,0.985,0.984,0.984,0.983,0.982,0.981,0.980,0.979,0.978,0.977,0.975,0.974,0.973,0.972,0.970,0.969,0.967,0.966,0.964,0.962,0.961,0.959,0.957,0.955,0.953,0.951,0.949,0.947,0.945,0.942,0.940,0.937,0.935,0.932,0.929,0.926,0.924,0.921,0.918,0.914,0.911,0.908,0.904,0.901,0.897,0.894,0.890,0.886,0.882,0.878,0.874,0.870,0.866,0.861,0.857,0.852,0.847,0.843,0.838,0.833,0.828,0.823,0.818,0.812,0.807,0.802,0.796,0.791,0.785,0.779,0.774,0.768,0.762,0.756,0.750,0.744,0.737,0.731,0.725,0.718,0.712,0.706,0.699,0.693,0.686,0.679,0.673,0.666,0.659,0.653,0.646,0.639,0.632,0.626,0.619,0.612,0.605,0.598,0.592,0.585,0.578,0.571,0.565,0.558,0.551,0.544,0.538,0.531,0.525,0.518,0.512,0.505,0.499,0.492,0.486,0.480,0.474,0.467,0.461,0.455,0.449,0.444,0.438,0.432,0.426,0.421,0.415,0.410,0.405,0.399,};
    // Double_t green[NRGBs] = {0.999,0.999,0.998,0.997,0.996,0.995,0.994,0.994,0.993,0.992,0.991,0.990,0.989,0.988,0.986,0.985,0.984,0.983,0.982,0.981,0.979,0.978,0.977,0.975,0.974,0.972,0.971,0.969,0.968,0.966,0.965,0.963,0.961,0.960,0.958,0.956,0.954,0.952,0.950,0.948,0.946,0.944,0.942,0.940,0.938,0.936,0.933,0.931,0.929,0.926,0.924,0.921,0.919,0.916,0.913,0.911,0.908,0.905,0.902,0.900,0.897,0.894,0.891,0.888,0.885,0.881,0.878,0.875,0.872,0.868,0.865,0.861,0.858,0.854,0.851,0.847,0.844,0.840,0.836,0.832,0.828,0.824,0.820,0.816,0.812,0.808,0.804,0.800,0.796,0.792,0.787,0.783,0.778,0.774,0.770,0.765,0.760,0.756,0.751,0.747,0.742,0.737,0.732,0.728,0.723,0.718,0.713,0.708,0.703,0.698,0.693,0.688,0.683,0.678,0.673,0.668,0.662,0.657,0.652,0.647,0.642,0.636,0.631,0.626,0.621,0.615,0.610,0.605,0.599,0.594,0.589,0.583,0.578,0.572,0.567,0.562,0.556,0.551,0.546,0.540,0.535,0.530,0.524,0.519,0.514,0.508,0.503,0.498,0.492,0.487,0.482,0.477,0.472,0.466,0.461,0.456,0.451,0.446,0.441,0.436,0.431,0.426,0.421,0.416,0.411,0.406,0.401,0.396,0.392,0.387,0.382,0.378,0.373,0.368,0.364,0.359,0.355,0.351,0.346,0.342,0.337,0.333,0.329,0.325,0.321,0.317,0.313,0.309,0.305,0.301,0.297,0.293,0.289,0.286,0.282,0.278,0.275,0.271,0.268,0.264,0.261,0.258,0.254,0.251,0.248,0.245,0.242,0.239,0.236,0.233,0.230,0.227,0.224,0.221,0.219,0.216,0.213,0.211,0.208,0.206,0.203,0.201,0.198,0.196,0.194,0.192,0.190,0.187,0.185,0.183,0.181,0.179,0.177,0.175,0.174,0.172,0.170,0.168,0.167,0.165,0.163,0.162,0.160,0.159,0.157,0.156,0.154,0.153,0.152,0.150,0.149,0.148,0.147,0.146,0.144,0.143,};
    // Double_t blue[NRGBs]  = {0.895,0.892,0.888,0.885,0.881,0.878,0.874,0.870,0.867,0.863,0.859,0.855,0.850,0.846,0.842,0.838,0.833,0.829,0.824,0.820,0.815,0.810,0.805,0.800,0.795,0.790,0.785,0.780,0.774,0.769,0.763,0.758,0.752,0.747,0.741,0.735,0.729,0.723,0.717,0.711,0.705,0.699,0.693,0.686,0.680,0.674,0.667,0.661,0.654,0.648,0.641,0.634,0.628,0.621,0.614,0.607,0.600,0.594,0.587,0.580,0.573,0.566,0.559,0.552,0.545,0.538,0.531,0.524,0.517,0.510,0.502,0.495,0.488,0.481,0.474,0.467,0.460,0.453,0.446,0.439,0.432,0.425,0.419,0.412,0.405,0.398,0.391,0.385,0.378,0.371,0.365,0.358,0.351,0.345,0.339,0.332,0.326,0.320,0.313,0.307,0.301,0.295,0.289,0.283,0.277,0.272,0.266,0.260,0.255,0.249,0.244,0.238,0.233,0.228,0.223,0.217,0.212,0.208,0.203,0.198,0.193,0.189,0.184,0.179,0.175,0.171,0.167,0.162,0.158,0.154,0.150,0.146,0.143,0.139,0.135,0.132,0.128,0.125,0.121,0.118,0.115,0.112,0.109,0.106,0.103,0.100,0.097,0.095,0.092,0.089,0.087,0.084,0.082,0.080,0.077,0.075,0.073,0.071,0.069,0.067,0.065,0.063,0.062,0.060,0.058,0.056,0.055,0.053,0.052,0.050,0.049,0.048,0.046,0.045,0.044,0.043,0.041,0.040,0.039,0.038,0.037,0.036,0.035,0.034,0.034,0.033,0.032,0.031,0.030,0.030,0.029,0.028,0.028,0.027,0.026,0.026,0.025,0.025,0.024,0.024,0.023,0.023,0.023,0.022,0.022,0.021,0.021,0.021,0.020,0.020,0.020,0.019,0.019,0.019,0.019,0.018,0.018,0.018,0.018,0.018,0.017,0.017,0.017,0.017,0.017,0.017,0.016,0.016,0.016,0.016,0.016,0.016,0.016,0.016,0.016,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.014,0.014,0.014,};

    // Double_t red[NRGBs]   = {0.237,0.229,0.222,0.216,0.210,0.205,0.201,0.197,0.195,0.193,0.191,0.190,0.190,0.190,0.191,0.192,0.194,0.196,0.199,0.203,0.206,0.210,0.215,0.220,0.225,0.231,0.237,0.243,0.250,0.257,0.264,0.272,0.279,0.287,0.295,0.304,0.313,0.321,0.330,0.339,0.349,0.358,0.368,0.378,0.387,0.397,0.407,0.417,0.427,0.438,0.448,0.458,0.469,0.479,0.489,0.500,0.510,0.520,0.531,0.541,0.551,0.562,0.572,0.582,0.592,0.602,0.612,0.622,0.632,0.642,0.652,0.661,0.671,0.680,0.690,0.699,0.708,0.717,0.726,0.734,0.743,0.751,0.760,0.768,0.776,0.784,0.792,0.800,0.807,0.815,0.822,0.829,0.836,0.843,0.849,0.856,0.862,0.868,0.874,0.880,0.886,0.891,0.897,0.902,0.907,0.912,0.917,0.922,0.926,0.930,0.935,0.939,0.942,0.946,0.950,0.953,0.957,0.960,0.963,0.966,0.968,0.971,0.973,0.976,0.978,0.980,0.982,0.984,0.985,0.987,0.988,0.990,0.991,0.992,0.993,0.994,0.994,0.995,0.996,0.996,0.996,0.997,0.997,0.997,0.997,0.997,0.997,0.996,0.996,0.996,0.995,0.994,0.994,0.993,0.992,0.991,0.991,0.990,0.989,0.987,0.986,0.985,0.984,0.983,0.981,0.980,0.979,0.977,0.976,0.974,0.973,0.971,0.970,0.968,0.966,0.965,0.963,0.961,0.959,0.958,0.956,0.954,0.952,0.951,0.949,0.947,0.945,0.943,0.941,0.939,0.937,0.935,0.933,0.931,0.929,0.927,0.925,0.923,0.921,0.919,0.917,0.915,0.913,0.911,0.908,0.906,0.904,0.902,0.899,0.897,0.895,0.892,0.890,0.887,0.885,0.882,0.879,0.876,0.874,0.871,0.868,0.865,0.862,0.858,0.855,0.852,0.848,0.844,0.841,0.837,0.833,0.829,0.824,0.820,0.815,0.810,0.805,0.800,0.795,0.790,0.784,0.778,0.772,0.765,0.759,0.752,0.745,0.737,0.730,0.722,0.713,0.705,0.696,0.687,0.677,0.667,};
    // Double_t green[NRGBs] = {0.327,0.335,0.342,0.350,0.358,0.366,0.373,0.381,0.389,0.397,0.405,0.412,0.420,0.428,0.436,0.444,0.452,0.460,0.468,0.476,0.484,0.492,0.500,0.508,0.516,0.524,0.531,0.539,0.547,0.555,0.563,0.571,0.579,0.586,0.594,0.602,0.609,0.617,0.625,0.632,0.640,0.647,0.655,0.662,0.670,0.677,0.684,0.691,0.699,0.706,0.713,0.720,0.727,0.734,0.740,0.747,0.754,0.760,0.767,0.773,0.780,0.786,0.792,0.799,0.805,0.811,0.817,0.822,0.828,0.834,0.839,0.845,0.850,0.856,0.861,0.866,0.871,0.876,0.881,0.885,0.890,0.894,0.899,0.903,0.907,0.911,0.915,0.919,0.923,0.927,0.930,0.934,0.937,0.940,0.943,0.946,0.949,0.951,0.954,0.956,0.959,0.961,0.963,0.965,0.967,0.969,0.970,0.972,0.973,0.974,0.975,0.976,0.977,0.977,0.978,0.978,0.979,0.979,0.979,0.979,0.978,0.978,0.977,0.977,0.976,0.975,0.974,0.973,0.971,0.970,0.968,0.966,0.965,0.963,0.960,0.958,0.956,0.953,0.951,0.948,0.945,0.942,0.939,0.935,0.932,0.928,0.924,0.921,0.917,0.913,0.908,0.904,0.900,0.895,0.890,0.885,0.880,0.875,0.870,0.865,0.859,0.854,0.848,0.843,0.837,0.831,0.825,0.818,0.812,0.806,0.799,0.793,0.786,0.779,0.772,0.765,0.758,0.751,0.744,0.736,0.729,0.721,0.714,0.706,0.698,0.691,0.683,0.675,0.667,0.659,0.650,0.642,0.634,0.626,0.617,0.609,0.600,0.592,0.583,0.574,0.566,0.557,0.548,0.539,0.530,0.522,0.513,0.504,0.495,0.486,0.477,0.468,0.459,0.450,0.441,0.432,0.423,0.414,0.405,0.396,0.387,0.378,0.369,0.360,0.351,0.343,0.334,0.325,0.316,0.308,0.299,0.290,0.282,0.273,0.265,0.257,0.248,0.240,0.232,0.224,0.216,0.208,0.200,0.192,0.185,0.177,0.170,0.163,0.155,0.148,0.141,0.134,0.128,0.121,0.115,0.108,};
    // Double_t blue[NRGBs]  = {0.633,0.640,0.646,0.652,0.658,0.665,0.671,0.677,0.683,0.689,0.695,0.701,0.707,0.713,0.718,0.724,0.730,0.736,0.741,0.747,0.752,0.758,0.763,0.769,0.774,0.780,0.785,0.790,0.795,0.800,0.806,0.811,0.816,0.821,0.826,0.830,0.835,0.840,0.845,0.849,0.854,0.859,0.863,0.868,0.872,0.877,0.881,0.885,0.889,0.894,0.898,0.902,0.906,0.910,0.914,0.917,0.921,0.925,0.928,0.932,0.935,0.939,0.942,0.945,0.948,0.951,0.954,0.957,0.960,0.962,0.965,0.967,0.970,0.972,0.974,0.976,0.978,0.979,0.981,0.982,0.984,0.985,0.986,0.987,0.987,0.988,0.988,0.989,0.989,0.989,0.988,0.988,0.987,0.987,0.986,0.984,0.983,0.982,0.980,0.978,0.976,0.974,0.971,0.969,0.966,0.963,0.960,0.956,0.953,0.949,0.945,0.941,0.937,0.932,0.928,0.923,0.918,0.913,0.907,0.902,0.896,0.891,0.885,0.879,0.872,0.866,0.860,0.853,0.847,0.840,0.833,0.826,0.819,0.812,0.805,0.798,0.790,0.783,0.776,0.768,0.761,0.753,0.745,0.738,0.730,0.723,0.715,0.707,0.700,0.692,0.684,0.677,0.669,0.661,0.654,0.646,0.639,0.631,0.624,0.617,0.609,0.602,0.595,0.588,0.580,0.573,0.566,0.560,0.553,0.546,0.539,0.533,0.526,0.519,0.513,0.507,0.500,0.494,0.488,0.482,0.476,0.470,0.464,0.459,0.453,0.448,0.442,0.437,0.431,0.426,0.421,0.416,0.411,0.406,0.401,0.397,0.392,0.387,0.383,0.378,0.374,0.370,0.365,0.361,0.357,0.353,0.349,0.346,0.342,0.338,0.334,0.331,0.327,0.324,0.321,0.317,0.314,0.311,0.308,0.305,0.302,0.299,0.296,0.293,0.290,0.288,0.285,0.282,0.280,0.278,0.275,0.273,0.270,0.268,0.266,0.264,0.262,0.260,0.258,0.256,0.254,0.252,0.251,0.249,0.247,0.246,0.244,0.242,0.241,0.240,0.238,0.237,0.236,0.234,0.233,0.232,};

    Double_t blue[NRGBs]   = {1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,1.000,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.999,0.998,0.998,0.998,0.998,0.998,0.998,0.998,0.998,0.997,0.997,0.997,0.997,0.997,0.996,0.996,0.996,0.996,0.995,0.995,0.995,0.994,0.994,0.994,0.993,0.993,0.992,0.992,0.991,0.991,0.990,0.990,0.989,0.989,0.988,0.987,0.986,0.986,0.985,0.984,0.983,0.982,0.981,0.980,0.979,0.978,0.977,0.976,0.975,0.974,0.972,0.971,0.969,0.968,0.966,0.965,0.963,0.961,0.960,0.958,0.956,0.954,0.952,0.950,0.947,0.945,0.943,0.940,0.938,0.935,0.932,0.930,0.927,0.924,0.921,0.918,0.915,0.911,0.908,0.904,0.901,0.897,0.893,0.890,0.886,0.882,0.877,0.873,0.869,0.864,0.860,0.855,0.851,0.846,0.841,0.836,0.831,0.825,0.820,0.815,0.809,0.804,0.798,0.792,0.787,0.781,0.775,0.769,0.762,0.756,0.750,0.743,0.737,0.731,0.724,0.717,0.711,0.704,0.697,0.690,0.683,0.676,0.669,0.662,0.655,0.648,0.641,0.633,0.626,0.619,0.612,0.604,0.597,0.590,0.582,0.575,0.568,0.560,0.553,0.546,0.539,0.531,0.524,0.517,0.510,0.502,0.495,0.488,0.481,0.474,0.467,0.460,0.454,0.447,0.440,0.433,0.427,0.420,0.414,0.407,0.401,0.395,0.389,0.382,0.376,0.371,0.365,0.359,0.353,};
    Double_t green[NRGBs] = {0.878,0.878,0.877,0.876,0.875,0.874,0.873,0.873,0.872,0.871,0.870,0.869,0.868,0.867,0.865,0.864,0.863,0.862,0.861,0.860,0.858,0.857,0.856,0.854,0.853,0.851,0.850,0.848,0.847,0.845,0.844,0.842,0.840,0.839,0.837,0.835,0.833,0.831,0.829,0.827,0.825,0.823,0.821,0.819,0.817,0.815,0.812,0.810,0.808,0.805,0.803,0.800,0.798,0.795,0.792,0.790,0.787,0.784,0.781,0.779,0.776,0.773,0.770,0.767,0.764,0.760,0.757,0.754,0.751,0.747,0.744,0.740,0.737,0.733,0.730,0.726,0.723,0.719,0.715,0.711,0.707,0.703,0.699,0.695,0.691,0.687,0.683,0.679,0.675,0.671,0.666,0.662,0.657,0.653,0.649,0.644,0.639,0.635,0.630,0.626,0.621,0.616,0.611,0.607,0.602,0.597,0.592,0.587,0.582,0.577,0.572,0.567,0.562,0.557,0.552,0.547,0.541,0.536,0.531,0.526,0.521,0.515,0.510,0.505,0.500,0.494,0.489,0.484,0.478,0.473,0.468,0.462,0.457,0.451,0.446,0.441,0.435,0.430,0.425,0.419,0.414,0.409,0.403,0.398,0.393,0.387,0.382,0.377,0.371,0.366,0.361,0.356,0.351,0.345,0.340,0.335,0.330,0.325,0.320,0.315,0.310,0.305,0.300,0.295,0.290,0.285,0.280,0.275,0.271,0.266,0.261,0.257,0.252,0.247,0.243,0.238,0.234,0.230,0.225,0.221,0.216,0.212,0.208,0.204,0.200,0.196,0.192,0.188,0.184,0.180,0.176,0.172,0.168,0.165,0.161,0.157,0.154,0.150,0.147,0.143,0.140,0.137,0.133,0.130,0.127,0.124,0.121,0.118,0.115,0.112,0.109,0.106,0.103,0.100,0.098,0.095,0.092,0.090,0.087,0.085,0.082,0.080,0.077,0.075,0.073,0.071,0.069,0.066,0.064,0.062,0.060,0.058,0.056,0.054,0.053,0.051,0.049,0.047,0.046,0.044,0.042,0.041,0.039,0.038,0.036,0.035,0.033,0.032,0.031,0.029,0.028,0.027,0.026,0.025,0.023,0.022,};
    Double_t red[NRGBs]  = {0.716,0.714,0.711,0.708,0.705,0.703,0.700,0.697,0.694,0.690,0.687,0.684,0.681,0.677,0.674,0.671,0.667,0.663,0.660,0.656,0.652,0.649,0.645,0.641,0.637,0.633,0.628,0.624,0.620,0.616,0.611,0.607,0.603,0.598,0.593,0.589,0.584,0.579,0.575,0.570,0.565,0.560,0.555,0.550,0.545,0.540,0.535,0.530,0.524,0.519,0.514,0.508,0.503,0.498,0.492,0.487,0.481,0.476,0.470,0.465,0.459,0.454,0.448,0.443,0.437,0.432,0.426,0.420,0.415,0.409,0.403,0.398,0.392,0.387,0.381,0.375,0.370,0.364,0.359,0.353,0.347,0.342,0.336,0.331,0.326,0.320,0.315,0.309,0.304,0.299,0.293,0.288,0.283,0.278,0.273,0.268,0.263,0.258,0.253,0.248,0.243,0.238,0.233,0.229,0.224,0.219,0.215,0.210,0.206,0.201,0.197,0.193,0.189,0.184,0.180,0.176,0.172,0.168,0.164,0.161,0.157,0.153,0.149,0.146,0.142,0.139,0.136,0.132,0.129,0.126,0.123,0.120,0.117,0.114,0.111,0.108,0.105,0.102,0.100,0.097,0.095,0.092,0.090,0.087,0.085,0.083,0.080,0.078,0.076,0.074,0.072,0.070,0.068,0.066,0.065,0.063,0.061,0.059,0.058,0.056,0.055,0.053,0.052,0.050,0.049,0.048,0.047,0.045,0.044,0.043,0.042,0.041,0.040,0.039,0.038,0.037,0.036,0.035,0.034,0.033,0.032,0.032,0.031,0.030,0.030,0.029,0.028,0.028,0.027,0.026,0.026,0.025,0.025,0.024,0.024,0.023,0.023,0.023,0.022,0.022,0.021,0.021,0.021,0.020,0.020,0.020,0.020,0.019,0.019,0.019,0.019,0.018,0.018,0.018,0.018,0.018,0.017,0.017,0.017,0.017,0.017,0.017,0.016,0.016,0.016,0.016,0.016,0.016,0.016,0.016,0.016,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.015,0.014,0.014,0.014,0.014,0.014,0.014,0.014,};

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}

void ViewWindow::PaletteGrayInv()
{
    gStyle->SetFrameFillColor(kWhite);
    const UInt_t Number = 2;
    Int_t nb=50;
    Double_t Red[Number]   = { 0.8, 0.0};
    Double_t Green[Number] = { 0.8, 0.0};
    Double_t Blue[Number]  = { 0.8, 0.0};
    Double_t Stops[Number] = { 0, 1.00};
    TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,nb);
    gStyle->SetNumberContours(nb);
}
