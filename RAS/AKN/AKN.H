/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::incompressible::RASModels::LamBremhorstKE

Group
    AKN

Description
    Abe, Kondoh and Nagano low-Reynolds number k-epsilon turbulence model
    for incompressible flows

SourceFiles
    AKN.C

\*---------------------------------------------------------------------------*/

#ifndef AKN_H
#define AKN_H

#include "RASModel.H"
#include "eddyViscosity.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RASModels
{

/*---------------------------------------------------------------------------*\
                             Class AKN Declaration
\*---------------------------------------------------------------------------*/

template<class BasicTurbulenceModel>
class AKN
:
    public eddyViscosity<RASModel<BasicTurbulenceModel>>
{
    // Private Member Functions

	// Disallow default bitwise copy construct and assignment
	AKN(const AKN&);
	void operator=(const AKN&);


protected:

    // Protected data

	// Model coefficients

	    dimensionedScalar Cmu_;
	    dimensionedScalar C1_;
	    dimensionedScalar C2_;
	    dimensionedScalar sigmaK_;
	    dimensionedScalar sigmaEps_;

	// Fields

	    volScalarField k_;
	    volScalarField epsilon_;

	//- Wall distance
	//  Note : Difference to wall distance in parent RASModel
	//  which is for near-wall cells only.
	const volScalarField& y_;


    // Protected Member Functions

	tmp<volScalarField> yStar_;
	tmp<volScalarField> Rt_;
	tmp<volScalarField> fMu_;
	tmp<volScalarField> f2_;

	virtual void correctNut();
	virtual tmp<fvScalarMatrix> kSource() const;
	virtual tmp<fvScalarMatrix> epsilonSource() const;


public:

    typedef typename BasicTurbulenceModel::alphaField alphaField;
    typedef typename BasicTurbulenceModel::rhoField rhoField;
    typedef typename BasicTurbulenceModel::transportModel transportModel;


    //- Runtime type information
    TypeName("AKN");


    // Constructors

	//- Construct from components
	AKN
	(
	    const alphaField& alpha,
	    const rhoField& rho,
	    const volVectorField& U,
	    const surfaceScalarField& alphaRhoPhi,
	    const surfaceScalarField& phi,
	    const transportModel& transport,
	    const word& propertiesName = turbulenceModel::propertiesName,
	    const word& type = typeName
	);


    //- Destructor
    virtual ~AKN()
    {}


    // Member Functions

	//- Re-read model coefficients if they have changed
	virtual bool read();

	//- Return the effective diffusivity for k
	tmp<volScalarField> DkEff() const
	{
	    return tmp<volScalarField>
	    (
		new volScalarField
		(
		    "DkEff",
		    (this->nut_/sigmak_ + this->nu())
		)
	    );
	}

	//- Return the effective diffusivity for epsilon
	tmp<volScalarField> DepsilonEff() const
	{
	    return tmp<volScalarFIeld>
	    (
		new volScalarField
		(
		    "DepsilonEff",
		    (this->nut_/sigmaEps_ + this->nu())
		)
	    );
	}

	//- Return the turbulence kinetic energy
	virtual tmp<volScalarField> k() const
	{
	    return k_;
	}

	//- Return the turbulence kinetic energy dissipation rate
	virtual tmp<volScalarField> epsilon() const
	{
	    return epsilon_;
	}

	//- Solve the turbulence equation and correct the turbulence viscosity
	virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "AKN.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
