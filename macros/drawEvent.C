void drawEvent()
{
    auto file = new TFile("data/two_proton_100.root");
    auto tree = (TTree*) file -> Get("event");
    //tree -> Print();

    // https://root.cern.ch/doc/master/classTTree.html

    //auto cvsAll = new TCanvas("cvsAll","cvsAll",800,650);
    auto cvsAll = new TCanvas("cvsAll","cvsAll",2000,2000);
    cvsAll -> Divide(2,2);

    //TCut cut = "MCStepTPC.fEdep";
    TCut cut = "MCStepTPC.fEdep*(Entry$==0)";
    cout << cut << endl;

    auto histXY = new TH2D("histXY","energy deposit (upstream view);X (mm);Y (mm)", 100,-50,50, 100,-50,50);
    histXY -> SetStats(0);
    auto cvsXY = cvsAll -> cd(1);
    cvsXY -> SetMargin(0.15,0.15,0.15,0.15);
    tree -> Draw("MCStepTPC.fY:MCStepTPC.fX>>histXY",cut,"colz");

    auto histZX = new TH2D("histZX","energy deposit (top view);Z (mm);X (mm)", 100,-50,50, 100,-50,50);
    histZX -> SetStats(0);
    auto cvsZX = cvsAll -> cd(2);
    cvsZX -> SetMargin(0.15,0.15,0.15,0.15);
    tree -> Draw("MCStepTPC.fX:MCStepTPC.fZ>>histZX",cut,"colz");
    histZX -> Draw("colz");

    auto histZY = new TH2D("histZY","energy deposit (side view);Z (mm);Y (mm)", 100,-50,50, 100,-50,50);
    histZY -> SetStats(0);
    auto cvsZY = cvsAll -> cd(3);
    cvsZY -> SetMargin(0.15,0.15,0.15,0.15);
    tree -> Draw("MCStepTPC.fY:MCStepTPC.fZ>>histZY",cut,"colz");
    histZY -> Draw("colz");

    auto histE = new TH1D("histE","energy deposit;R (mm);energy deposit (MeV)", 100,0,100);
    histE -> SetStats(0);
    auto cvsE = cvsAll -> cd(4);
    cvsE -> SetMargin(0.15,0.15,0.15,0.15);
    tree -> Draw("sqrt(MCStepTPC.fX*MCStepTPC.fX+MCStepTPC.fY*MCStepTPC.fY+MCStepTPC.fZ*MCStepTPC.fZ)>>histE",cut,"colz");
    histE -> Draw("hist");
}
