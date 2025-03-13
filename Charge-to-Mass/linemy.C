{
    const int ndata = 4;

   
    float x[ndata] = {4.41e-10, 9.92e-10, 1.76e-09, 2.76e-09};
    float sx[ndata] = {1.25e-10, 1.88e-10, 2.51e-10, 3.14e-10};
    float y[ndata] = {146.0, 210.0, 338.0, 492.0};
    float sy[ndata] = {2.0, 2.0, 2.0, 2.0}; 

    TGraphErrors *mygraph = new TGraphErrors(ndata,x,y,sx,sy);
    mygraph->Draw("A*");
    gStyle->SetOptFit(1111);
    TF1 *fnew = new TF1("fnew","[0]*x+[1]",0,6);
    fnew->SetParameters(3.1416,0.7182); // arbitrary starting parameters
    mygraph->Fit(fnew);
}