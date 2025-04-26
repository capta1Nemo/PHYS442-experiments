#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
#include <TStyle.h>
#include <TFile.h>
{
    // Set global style options for fitting
    gStyle->SetOptFit(1111);

    // Create a canvas with 2x2 division
    TCanvas *c = new TCanvas("c", "Canvas", 1600, 1200);
    c->Divide(2, 2);

    // Step 1: Read data from the text file
    std::ifstream dataFile("output.txt");  // Open the text file for reading
    if (!dataFile.is_open()) {
        std::cerr << "Error: Unable to open file 'output.txt'!" << std::endl;
        return;
    }

    // Vector to hold the data (each row is a vector of doubles)
    std::vector<std::vector<double>> data;
    std::string line;

    // Read each line of the file
    while (std::getline(dataFile, line)) {
        std::stringstream ss(line);  // Create a stringstream for each line
        std::vector<double> row;     // A vector to store values for each line
        double val;
        
        // Extract each value from the line and push it into the row vector
        while (ss >> val) {
            row.push_back(val);
        }

        // Push the row (vector of doubles) into the data vector
        data.push_back(row);
    }

    // Create a histogram list to store histograms for later use
    std::vector<TH1F*> hist_list;

    // Loop over the columns (first 4 columns in the file)
    for (int i = 0; i < 4; ++i) {
        std::vector<double> col1_data;
        for (size_t j = 0; j < data.size(); ++j) {
            col1_data.push_back(data[j][i]);
        }

        c->cd(i + 1);  // Select the correct pad for drawing
        
        // Create a histogram
        TH1F *hist = new TH1F("hist0", "Histogram", 8, *std::min_element(col1_data.begin(), col1_data.end()), *std::max_element(col1_data.begin(), col1_data.end()));
        hist->SetFillColorAlpha(4, 0.4);  // Blue fill with 40% transparency
        hist->SetLineColor(1);            // Black border
        hist->SetLineWidth(2);
        hist->SetFillStyle(1001);         // Solid fill style
        
        // Fill histogram with data
        for (size_t j = 0; j < col1_data.size(); ++j) {
            hist->Fill(col1_data[j]);
        }

        // Fit a Gaussian to the histogram
        TF1 *gauss_fit = new TF1("gauss_fit", "[0]*exp(-0.5*((x-[1])/[2])**2)", *std::min_element(col1_data.begin(), col1_data.end()), *std::max_element(col1_data.begin(), col1_data.end()));
        gauss_fit->SetParameters(1, (*std::min_element(col1_data.begin(), col1_data.end()) + *std::max_element(col1_data.begin(), col1_data.end())) / 2.0, 4);
        gauss_fit->SetParName(0, "GAmplitude");
        gauss_fit->SetParName(1, "GMean");
        gauss_fit->SetParName(2, "GSigma");
        hist->Fit(gauss_fit, "S");  // Fit Gaussian

        // Fit a Poisson distribution to the histogram
        TF1 *poisson_fit = new TF1("poisson_fit", "[0] * TMath::Poisson(x, [1])", 0, *std::max_element(col1_data.begin(), col1_data.end()));
        poisson_fit->SetParameters(1, (*std::min_element(col1_data.begin(), col1_data.end()) + *std::max_element(col1_data.begin(), col1_data.end())) / 2.0);
        poisson_fit->SetParName(0, "PAmplitude");
        poisson_fit->SetParName(1, "PLambda");

        hist->Fit(poisson_fit, "S"); // Fit Poisson

        // Draw both fits on the same histogram
        gauss_fit->Draw("same");
        poisson_fit->Draw("same");

        hist_list.push_back(hist);  // Store the histogram
        hist->Draw();  // Draw histogram
        c->Update();
    }
}
