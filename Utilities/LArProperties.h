#ifndef LARPROPERTIES_H
#define LARPROPERTIES_H

#include <vector>

class LArProperties {
public:

    LArProperties();
    virtual ~LArProperties();

    //  methods
    void Init(); 
    void PrintInfo();

    double Density() { return Density(Temperature()); }
    double Density(double temperature);

    double DriftVelocity() { return DriftVelocity(Efield(), Temperature()); }
    double DriftVelocity(double efield, double temperature);
    
    double Eloss(double mom, double mass, double tcut);
    double ElossVar(double mom, double mass);
    double BirksCorrection(double dQdX);
    double ModBoxCorrection(double dQdX);

    // getters
    double Efield(unsigned int planegap=0) { return fEfield.at(planegap); }
    double Temperature()  { return fTemperature; }
    double ElectronLifetime()  { return fElectronlifetime; }
    double RadiationLength()  { return fRadiationLength; }

    std::vector<double> fEfield;           ///< kV/cm 
    double              fTemperature;      ///< kelvin
    double              fElectronlifetime; ///< microseconds
    double              fRadiationLength;  ///< g/cm^2

    // Following parameters are for use in Bethe-Bloch formula for dE/dx.
    double fZ;                ///< Ar atomic number
    double fA;                ///< Ar atomic mass (g/mol)
    double fI;                ///< Ar mean excitation energy (eV)
    double fSa;               ///< Sternheimer parameter a
    double fSk;               ///< Sternheimer parameter k
    double fSx0;              ///< Sternheimer parameter x0
    double fSx1;              ///< Sternheimer parameter x1
    double fScbar;            ///< Sternheimer parameter Cbar
};

#endif