#include "TaxStategy.h"


double TaxStategy::insuranceAndHousingFundSum(const double amount)
{
	double sum = 0;
	sum += this->insuranceAndHousingFund.endowmentInsuranceRatio * amount;
	sum += this->insuranceAndHousingFund.housingFundRatio * amount;
	sum += this->insuranceAndHousingFund.injuryInsuranceRatio * amount;
	sum += this->insuranceAndHousingFund.maternityInsuranceRatio * amount;
	sum += this->insuranceAndHousingFund.medicalInsuranceRatio * amount;
	sum += this->insuranceAndHousingFund.unemploymentInsuranceRatio * amount;

	return sum;
}

double TaxStategy::specialAdditionalDedcutionSum(const double amount)
{
	double sum = 0;
	sum += this->specialAdditionalDedcution.adultEducationDedcution;
	sum += this->specialAdditionalDedcution.childrensEducationDedcution;
	sum += this->specialAdditionalDedcution.careForElderlyDedcution;
	sum += this->specialAdditionalDedcution.housingLoansInterestDedcution;
	sum += this->specialAdditionalDedcution.HousingRentsDedcution;
	sum += this->specialAdditionalDedcution.treatmentForSeriousDiseaseDedcution;

	return sum;
}
