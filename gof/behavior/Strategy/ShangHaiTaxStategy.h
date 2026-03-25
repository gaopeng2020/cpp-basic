#pragma once
#include "TaxStategy.h"
class ShangHaiTaxStategy : public TaxStategy
{
public:
	virtual double calculateTax(const double amount, uint8_t mounth,
		InsuranceAndHousingFund insuranceAndHousingFund, SpecialAdditionalDedcution specialAdditionalDedcution) override;
};

