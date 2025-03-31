{
    const int ndata = 4;

    float x[ndata] = {     2.5,  3,  3.5,  4};
    float y[ndata] = {0.00955,  0.01003, 0.01145, 0.01000};
    float sy[ndata]= {0.00010,  0.0010,  0.0009,  0.00007};
    TGraphErrors *mygraph = new TGraphErrors(ndata, x, y, 0, sy);
    TString title = TString::Format("Fit for 5 squeeze");
    mygraph->SetTitle(title);
    mygraph->Draw("A*");
    gStyle->SetOptFit(1111);
    TF1 *fnew = new TF1("fnew","[0]*x+[1]",2.5,4);
    fnew->SetParameters(3.1416,2.7182); // arbitrary starting parameters
    mygraph->Fit(fnew);
    mygraph->GetXaxis()->SetLimits(2.4, 4.1);

}
