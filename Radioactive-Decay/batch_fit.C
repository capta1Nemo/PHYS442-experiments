void batch_fit(const char* inputFolder = "./data", const char* outputFolder = "./results") {
    // Create a system directory object to list files
    TSystemDirectory dir(inputFolder, inputFolder);
    TList *files = dir.GetListOfFiles();
    if (!files) {
        cout << "No files found in directory: " << inputFolder << endl;
        return;
    }

    // Loop over files
    TIter next(files);
    TSystemFile *file;
    while ((file = (TSystemFile*)next())) {
        TString filename = file->GetName();

        // Process only .txt files
        if (!filename.EndsWith(".txt")) continue;

        // Construct full file path
        TString filepath = TString(inputFolder) + "/" + filename;
        cout << "Processing: " << filepath << endl;

        // Read the data file into a graph
        TGraphErrors *mygraph = new TGraphErrors(filepath);
        mygraph->SetTitle("Exponential fit to T_i to 1/s_i");

        // Create a fitting function
        TF1 *expo_fit = new TF1("expo_fit","[0]*exp([1]*x)",0,250);
        expo_fit->SetParameters(100, -0.02);
        expo_fit->SetLineColor(kRed);
        expo_fit->SetLineWidth(2);

        // Perform fitting
        mygraph->Fit(expo_fit, "R");

        // Set axis labels
        mygraph->GetXaxis()->SetTitle("xdata (units)");
        mygraph->GetYaxis()->SetTitle("ydata (units)");
        mygraph->GetXaxis()->SetLimits(0, 250);
        mygraph->SetMinimum(0);
        mygraph->SetMaximum(0.4);

        // Create and draw canvas
        TCanvas *c1 = new TCanvas();
        mygraph->Draw("A*");

        // Construct output filename
        TString outputFilename = TString(outputFolder) + "/" + filename.ReplaceAll(".txt", ".png");

        // Save the canvas as a PNG file
        c1->SaveAs(outputFilename);
        
        // Clean up memory
        delete mygraph;
        delete expo_fit;
        delete c1;
    }

    cout << "Processing complete. Check the output folder: " << outputFolder << endl;
}
