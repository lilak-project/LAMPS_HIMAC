#include <cmath>
#include <fstream>
#include "TMath.h"
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
using namespace std;

void CalculateMomentum(double energy, double angle, double &px, double &py, double &pz);

void makeGenFile()
{
    TString dummyC;
    int     dummyI;
    int     eventID;
    double  dummyD;

    int     numParticles;
    int     pdg;
    int     ip;
    double  px;
    double  py;
    double  pz;
    double  phi_deg;
    double  theta_deg;

    auto file = new TFile("summary.root","recreate");
    auto tree = new TTree("particle","");
    tree -> Branch("ip"   ,&ip);
    tree -> Branch("px"   ,&px);
    tree -> Branch("py"   ,&py);
    tree -> Branch("pz"   ,&pz);
    tree -> Branch("phi"  ,&phi_deg);
    tree -> Branch("theta",&theta_deg);

    //int numEvents = 10000;
    int numEvents = 100;

    ifstream file_input("P1_P2_sim.out");
    ofstream file_gen(Form("two_proton_%d.gen",numEvents));

    file_gen << "p" << endl;
    file_gen << numEvents << endl;

    eventID = 0;
    numParticles = 2;
    pdg = 2212;
    double vertexX = 0;
    double vertexY = 0;
    double vertexZ = 0;
    double e1, a1, px1, py1, pz1;
    double e2, a2, px2, py2, pz2;

    int countEvents = 0;
    while(file_input >> e1 >> a1 >> e2 >> a2)
    {
        CalculateMomentum(e1,a1,px1,py1,pz1);
        CalculateMomentum(e2,a2,px2,py2,pz2);

        file_gen << eventID << " " << numParticles << " " << vertexX << " " << vertexY << " " << vertexZ << endl;
        file_gen << pdg << " " << px1 << " " << py1 << " " << pz1 << endl;
        file_gen << pdg << " " << px2 << " " << py2 << " " << pz2 << endl;
        eventID++;

        TVector3 mom1(px1,py1,pz1);
        ip = 0;
        px = mom1.x();
        py = mom1.y();
        pz = mom1.z();
        theta_deg = mom1.Theta()*TMath::RadToDeg();
        phi_deg   = mom1.Phi()  *TMath::RadToDeg();
        tree -> Fill();

        TVector3 mom2(px2,py2,pz2);
        ip = 1;
        px = mom2.x();
        py = mom2.y();
        pz = mom2.z();
        theta_deg = mom2.Theta()*TMath::RadToDeg();
        phi_deg   = mom2.Phi()  *TMath::RadToDeg();
        tree -> Fill();

        countEvents++;
        if (countEvents==numEvents)
            break;
    }

    auto cvs = new TCanvas("cvs","",1200,700);
    cvs -> Divide(2,2);
    cvs -> cd(1); tree -> Draw("theta>>theta0(180,0,180)","ip==0");
    cvs -> cd(2); tree -> Draw("theta>>theta1(180,0,180)","ip==1");
    cvs -> cd(3); tree -> Draw("phi>>phi0(360,0,360)","ip==0");
    cvs -> cd(4); tree -> Draw("phi>>phi1(360,0,360)","ip==1");

    file -> cd();
    tree -> Write();
}

void CalculateMomentum(double energy, double angle, double &px, double &py, double &pz)
{
    double mass = 938.272;
    double momentum = sqrt( (energy + mass)*(energy + mass) - mass*mass );

    double theta = angle * TMath::DegToRad();
    double phi_range = 6.0 * TMath::DegToRad();
    double phi = gRandom -> Uniform(-phi_range, phi_range);

    px = momentum * TMath::Sin(theta) * TMath::Cos(phi);
    py = momentum * TMath::Sin(theta) * TMath::Sin(phi);
    pz = momentum * TMath::Cos(theta);
}
