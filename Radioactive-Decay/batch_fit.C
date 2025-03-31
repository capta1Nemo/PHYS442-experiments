void batch_fit(const char* inputFolder = "./data", const char* outputFolder = "./results") {
    TSystemDirectory dir(inputFolder, inputFolder);
    TList *files = dir.GetListOfFiles();
    if (!files) {
        cout << "No files found in directory: " << inputFolder << endl;
        return;
    }

    // Open a results file to store p1 values
    TString resultsFile = TString(outputFolder) + "/fit_results.txt";
    ofstream results(resultsFile.Data(), ios::out);
    if (!results) {
        cout << "Error: Unable to open results file for writing!" << endl;
        return;
    }

    results << "Filename\tlambda_Value" << endl; // Header

    TIter next(files);
    TSystemFile *file;
    while ((file = (TSystemFile*)next())) {
        TString filename = file->GetName();

        if (!filename.EndsWith(".txt")) continue;

        TString filepath = TString(inputFolder) + "/" + filename;
        cout << "Processing: " << filepath << endl;

        TGraphErrors *mygraph = new TGraphErrors(filepath);
        TString title = TString::Format("Fit for %s", filename.Data());
        title.ReplaceAll(".txt", ""); 
        mygraph->SetTitle(title);

        TF1 *expo_fit = new TF1("expo_fit","[0]*exp(-[1]*x)");
        expo_fit->SetParameters(0.3, -0.01);
        expo_fit->SetLineColor(kRed);
        expo_fit->SetLineWidth(2);

        mygraph->Fit(expo_fit);
        mygraph->GetXaxis()->SetTitle("T_{i} \\ (\\text{seconds})");
        mygraph->GetYaxis()->SetTitle("1/s_{i} \\ (\\text{seconds}^{-1})");

        // Extract and store the lambda value
        double p1 = expo_fit->GetParameter(1);
        double p1_err = expo_fit->GetParError(1);
        results << filename.Data() << "\t" << fixed << setprecision(5) << p1 << "\t" << setprecision(5) << p1_err << endl;


        TCanvas *c1 = new TCanvas();
        mygraph->Draw("A*");
        gStyle->SetOptFit(1111);

        TString outputFilename = TString(outputFolder) + "/" + filename.ReplaceAll(".txt", ".png");
        c1->SaveAs(outputFilename);
    
        delete mygraph;
        delete expo_fit;
        delete c1;
    }
    
    results.close();
    cout << "Processing complete. Check the output folder: " << outputFolder << endl;
}
