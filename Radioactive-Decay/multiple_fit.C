// Example multi-fitting macro written for ROOT
{
   // Defining a Canvas
   // If not defined, the plot will be drawn on the previous canvas on each execution

   // Defining the graph object with error bars. Read from a txt file. Set it to your directory
   TGraphErrors *mygraph = new TGraphErrors("./data/3.0-5.txt");
   // Setting the title of the graph
   mygraph->SetTitle("Exponential fit of T_i to 1/s_i");

   // Create a fitting function
   TF1 *expo_fit = new TF1("expo_fit","[0]*exp([1]*x)");
   expo_fit->SetParameters(0.3, -0.01);
   expo_fit->SetLineColor(kRed);
   expo_fit->SetLineWidth(2);

   // Perform fitting
   mygraph->Fit(expo_fit);

   // Set axis labels
   mygraph->GetXaxis()->SetTitle("xdata (units)");
   mygraph->GetYaxis()->SetTitle("ydata (units)");
   TCanvas *c1 = new TCanvas();
   mygraph->Draw("A*");
   gStyle->SetOptFit(1111);
}
