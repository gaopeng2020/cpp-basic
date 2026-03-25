#include "ShangHaiTaxStategy.h"

double ShangHaiTaxStategy::calculateTax(const double amount, uint8_t mounth,
	InsuranceAndHousingFund insuranceAndHousingFund, SpecialAdditionalDedcution specialAdditionalDedcution)
{
	TaxStategy::insuranceAndHousingFund = insuranceAndHousingFund;
	TaxStategy::specialAdditionalDedcution = specialAdditionalDedcution;
	double taxThreshod = 5000;
	double taxPaymentSum = 0;
	double currentMounthPayTaxs = 0;

	if (mounth > 12) {
		mounth = 12;
	}
	else if (mounth < 1) {
		mounth = 1;
	}
	for (size_t i = 1; i <= mounth; i++)
	{
		double insuranceHousingFundDedcution = TaxStategy::insuranceAndHousingFundSum(amount);
		double specialAdditionalDedcution = TaxStategy::specialAdditionalDedcutionSum(amount);

		double incoms2PayTax = (amount - taxThreshod - insuranceHousingFundDedcution - specialAdditionalDedcution) * i;
		double taxRatio = 0;
		unsigned quickDeduction = 0;
		if (incoms2PayTax <= 36000) {
			taxRatio = 0.03;
			quickDeduction = 0;
		}
		else if (incoms2PayTax <= 144000) {
			taxRatio = 0.1;
			quickDeduction = 2520;
		}
		else if (incoms2PayTax <= 300000) {
			taxRatio = 0.2;
			quickDeduction = 16920;
		}
		else if (incoms2PayTax <= 420000) {
			taxRatio = 0.25;
			quickDeduction = 31920;
		}
		else if (incoms2PayTax <= 660000) {
			taxRatio = 0.3;
			quickDeduction = 52920;
		}
		else if (incoms2PayTax <= 960000) {
			taxRatio = 0.35;
			quickDeduction = 85920;
		}
		else
		{
			taxRatio = 0.45;
			quickDeduction = 181920;
		}

		currentMounthPayTaxs = incoms2PayTax * taxRatio - quickDeduction - taxPaymentSum;
		taxPaymentSum += currentMounthPayTaxs;
	}


	return currentMounthPayTaxs;
}
