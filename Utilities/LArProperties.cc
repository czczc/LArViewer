#include "LArProperties.h"

#include <iostream>
using namespace std;

LArProperties::LArProperties()
{
    Init();
}

LArProperties::~LArProperties()
{}

void LArProperties::Init()
{
    // LarData/Utilities/larproperties.fcl
    // http://pdg.lbl.gov/AtomicNuclearProperties/
    double e[3] = {0.5, 0.666, 0.8};
    for (int i=0; i!=3; i++) {
        fEfield.push_back(e[i]);
    }
         fTemperature = 87;     // K
    fElectronlifetime = 3.0e3;  // microseconds
     fRadiationLength = 19.55;  // g/cm^2
    // Following parameters are for use in Bethe-Bloch formula for dE/dx.
        fZ = 18;                ///< Ar atomic number
        fA = 39.948;            ///< Ar atomic mass (g/mol)
        fI = 188.00;            ///< Ar mean excitation energy (eV)
       fSa = 0.1956;            ///< Ar Sternheimer parameter a
       fSk = 3.0000;            ///< Ar Sternheimer parameter k
      fSx0 = 0.2000;            ///< Ar Sternheimer parameter x0
      fSx1 = 3.0000;            ///< Ar Sternheimer parameter x1
    fScbar = 5.2146;            ///< Ar Sternheimer parameter Cbar
}

double LArProperties::Density(double temperature)
{
    // temperature is assumed to be in degrees Kelvin
    // density is nearly a linear function of temperature.  see the NIST tables for details
    // slope is between -6.2 and -6.1, intercept is 1928 kg/m^3
    // this parameterization will be good to better than 0.5%.
    // density is returned in g/cm^3
    double density = -0.00615*temperature + 1.928;
    return density;
}

//----------------------------------------------------------------------------------
// Drift Velocity as a function of Electric Field and LAr Temperature
// from : W. Walkowiak, NIM A 449 (2000) 288-294
//
// Efield should have units of kV/cm
// Temperature should have units of Kelvin
//----------------------------------------------------------------------------------
double LArProperties::DriftVelocity(double efield, double temperature)
{
    if(efield > 4.0) {
        cout << "Warning: E-field value of " << efield 
             << " kV/cm is outside of range covered by drift velocity parameterization.";
    }
    if(temperature < 87.0 || temperature > 94.0) {
        cout << "Warning: Temperature value of " << temperature 
             << " K is outside of range covered by drift velocity parameterization.";

    }

    double tshift = -87.203+temperature;
    double xFit = 0.0938163-0.0052563*tshift-0.0001470*tshift*tshift;
    double uFit = 5.18406+0.01448*tshift-0.003497*tshift*tshift-0.000516*tshift*tshift*tshift;
    double vd;

    // Icarus Parameter Set, use as default
    double  P1 = -0.04640; // K^-1
    double  P2 = 0.01712;  // K^-1
    double  P3 = 1.88125;  // (kV/cm)^-1
    double  P4 =  0.99408; // kV/cm
    double  P5 =  0.01172; // (kV/cm)^-P6
    double  P6 =  4.20214;
    double  T0 =  105.749; // K
    // Walkowiak Parameter Set
    double  P1W = -0.01481;
    double  P2W = -0.0075; 
    double  P3W =  0.141;  
    double  P4W =  12.4;   
    double  P5W =  1.627;  
    double  P6W =  0.317;
    double  T0W =  90.371; 

    // From Craig Thorne . . . currently not documented
    // smooth transition from linear at small fields to 
    // icarus fit at most fields to Walkowiak at very high fields
    if (efield < xFit) { vd=efield*uFit; }
    else if (efield<0.619) { 
        vd = ((P1*(temperature-T0)+1)
               *(P3*efield*log(1+P4/efield) + P5*pow(efield,P6))
               +P2*(temperature-T0));
    }
    else if (efield<0.699) {
        vd = 12.5*(efield-0.619)*((P1W*(temperature-T0W)+1)
               *(P3W*efield*log(1+P4W/efield) + P5W*pow(efield,P6W))
               +P2W*(temperature-T0W))+
             12.5*(0.699-efield)*((P1*(temperature-T0)+1)
               *(P3*efield*log(1+P4/efield) + P5*pow(efield,P6))
               +P2*(temperature-T0));
       }
    else {
        vd = ((P1W*(temperature-T0W)+1)
               *(P3W*efield*log(1+P4W/efield) + P5W*pow(efield,P6W))
               +P2W*(temperature-T0W));     
    }

    vd /= 10.;

    return vd; // in cm/us    
}

//----------------------------------------------------------------------------------
// Restricted mean energy loss (dE/dx) in units of MeV/cm.
//
// For unrestricted mean energy loss, set tcut = 0, or tcut large.
//
// Arguments:
//
// mom  - Momentum of incident particle in GeV/c.
// mass - Mass of incident particle in GeV/c^2.
// tcut - Maximum kinetic energy of delta rays (MeV).
//
// Returned value is positive.
//
// Based on Bethe-Bloch formula as contained in particle data book.
// Material parameters (stored in larproperties.fcl) are taken from
// pdg web site http://pdg.lbl.gov/AtomicNuclearProperties/
//----------------------------------------------------------------------------------
double LArProperties::Eloss(double mom, double mass, double tcut)
{
    // Some constants.
    double K = 0.307075;     // 4 pi N_A r_e^2 m_e c^2 (MeV cm^2/mol).
    double me = 0.510998918; // Electron mass (MeV/c^2).

    // Calculate kinematic quantities.
    double bg = mom / mass;           // beta*gamma.
    double gamma = sqrt(1. + bg*bg);  // gamma.
    double beta = bg / gamma;         // beta (velocity).
    double mer = 0.001 * me / mass;   // electron mass / mass of incident particle.
    double tmax = 2.*me* bg*bg / (1. + 2.*gamma*mer + mer*mer);  // Maximum delta ray energy (MeV).
    // Make sure tcut does not exceed tmax.
    if(tcut == 0. || tcut > tmax) { tcut = tmax; }

    // Calculate density effect correction (delta).
    double x = log10(bg);
    double delta = 0.;
    if(x >= fSx0) {
        delta = 2. * log(10.) * x - fScbar;
        if(x < fSx1) {
            delta += fSa * pow(fSx1 - x, fSk);
        }
    }

    // Calculate stopping number.
    double B = 0.5 * log(2.*me*bg*bg*tcut / (1.e-12 * fI*fI))
      - 0.5 * beta*beta * (1. + tcut / tmax) - 0.5 * delta;
    // Don't let the stopping number become negative.
    if(B < 1.) B = 1.;

    // Calculate dE/dx.
    double dedx = Density() * K*fZ*B / (fA * beta*beta);
    return dedx;   // MeV/cm.
}

//----------------------------------------------------------------------------------
// Energy loss fluctuation (sigma_E^2 / length in MeV^2/cm).
//
// Arguments:
//
// mom  - Momentum of incident particle in GeV/c.
//
// Based on Bichsel formula referred to but not given in pdg.
//----------------------------------------------------------------------------------
double LArProperties::ElossVar(double mom, double mass)
{
    // Some constants.
    double K = 0.307075;     // 4 pi N_A r_e^2 m_e c^2 (MeV cm^2/mol).
    double me = 0.510998918; // Electron mass (MeV/c^2).

    // Calculate kinematic quantities.
    double bg = mom / mass;          // beta*gamma.
    double gamma2 = 1. + bg*bg;      // gamma^2.
    double beta2 = bg*bg / gamma2;   // beta^2.

    // Calculate final result.
    double result = gamma2 * (1. - 0.5 * beta2) * me * (fZ / fA) * K * Density();
    return result;
}


//----------------------------------------------------------------------------------
// The below function assumes that the user has applied the lifetime correction and
// effective pitch between the wires (usually after 3D reconstruction). Using with
// mean wire pitch will not give correct results.
// parameters:
//  dQdX in electrons/cm, charge (amplitude or integral obtained) divided by effective pitch for a given 3D track.
// returns dEdX in MeV/cm
//----------------------------------------------------------------------------------
double LArProperties::BirksCorrection(double dQdx)
{
    // Correction for charge quenching using parameterization from
    // S.Amoruso et al., NIM A 523 (2004) 275
    // defined in "LarCore/SimpleTypesAndConstants/PhysicalConstants.h"
    double  A3t    = 0.8;         // see Nucl.Instrum.Meth.A523:275-286,2004
    double  K3t    = 0.0486;      // in g/(MeVcm^{2})*(kV/cm)
    double  rho    = Density();   // LAr density in g/cm^3
    double Wion    = 23.6e-6;     // 23.6 eV = 1e, Wion in MeV/e
    double    E    = Efield();    // Electric Field in the drift region in KV/cm
    K3t           /= rho;         // KV/MeV
    double dEdx    = dQdx/(A3t/Wion-K3t/E*dQdx);    //MeV/cm
    return dEdx;
}

//----------------------------------------------------------------------------------
// Modified Box model correction 
//----------------------------------------------------------------------------------
double LArProperties::ModBoxCorrection(double dQdx)
{
    // Modified Box model correction has better behavior than the Birks
    // correction at high values of dQ/dx.
    // defined in "LarCore/SimpleTypesAndConstants/PhysicalConstants.h"
    double rho     = Density();   // LAr density in g/cm^3
    double Wion    = 23.6e-6;     // 23.6 eV = 1e, Wion in MeV/e
    double E       = Efield();    // Electric Field in the drift region in KV/cm
    double Beta    = 0.212;       // in g/(MeVcm^{2})*(kV/cm)
    Beta          /= (rho * E);   // cm/MeV
    double Alpha   = 0.930;       // Modified Box Alpha (ArgoNeuT JINST)
    double dEdx    = (exp(Beta * Wion * dQdx ) - Alpha) / Beta;
    return dEdx;
}


//----------------------------------------------------------------------------------
void LArProperties::PrintInfo()
{
    cout << "       E Field [kV/cm]: " << Efield(0) << ", " << Efield(1) << ", " << Efield(2) << endl; 
    cout << "       Temperature [K]: " << Temperature() << endl;
    cout << "Drift Velocity [cm/us]: " << DriftVelocity() << endl;
    cout << "Electron Lifetime [us]: " << ElectronLifetime() << endl;
    cout << "        Density [g/cc]: " << Density() << endl;
    cout << " Radiation Length [cm]: " << RadiationLength()/Density() << endl;
    cout << "dE/dx 1GeV mu [MeV/cm]: " << Eloss(1, 0.10565837, 0) << endl;
    cout << " dE Fluctu. [MeV^2/cm]: " << ElossVar(1, 0.10565837) << endl;
    cout << "   Rc Birks @ 2 MeV/cm: " << 2/BirksCorrection(2e6/23.6) << endl;
    cout << "     Rc Box @ 2 MeV/cm: " << 2/ModBoxCorrection(2e6/23.6) << endl;
}
