#include <DB/AggregateFunctions/AggregateFunctionCount.h>
#include <DB/AggregateFunctions/AggregateFunctionSum.h>
#include <DB/AggregateFunctions/AggregateFunctionAvg.h>
#include <DB/AggregateFunctions/AggregateFunctionAny.h>
#include <DB/AggregateFunctions/AggregateFunctionAnyLast.h>
#include <DB/AggregateFunctions/AggregateFunctionUniq.h>
#include <DB/AggregateFunctions/AggregateFunctionGroupArray.h>
#include <DB/AggregateFunctions/AggregateFunctionsMinMax.h>
#include <DB/AggregateFunctions/AggregateFunctionQuantile.h>
#include <DB/AggregateFunctions/AggregateFunctionQuantileTiming.h>

#include <DB/AggregateFunctions/AggregateFunctionFactory.h>

#include <DB/DataTypes/DataTypeDate.h>
#include <DB/DataTypes/DataTypeDateTime.h>
#include <DB/DataTypes/DataTypeString.h>
#include <DB/DataTypes/DataTypeFixedString.h>


namespace DB
{


AggregateFunctionFactory::AggregateFunctionFactory()
{
}


/** Создать агрегатную функцию с числовым типом в параметре шаблона, в зависимости от типа аргумента.
  */
template<template <typename> class AggregateFunctionTemplate>
static IAggregateFunction * createWithNumericType(const IDataType & argument_type)
{
	     if (dynamic_cast<const DataTypeUInt8 	*>(&argument_type))	return new AggregateFunctionTemplate<UInt8>;
	else if (dynamic_cast<const DataTypeUInt16 	*>(&argument_type))	return new AggregateFunctionTemplate<UInt16>;
	else if (dynamic_cast<const DataTypeUInt32 	*>(&argument_type))	return new AggregateFunctionTemplate<UInt32>;
	else if (dynamic_cast<const DataTypeUInt64 	*>(&argument_type))	return new AggregateFunctionTemplate<UInt64>;
	else if (dynamic_cast<const DataTypeInt8 	*>(&argument_type))	return new AggregateFunctionTemplate<Int8>;
	else if (dynamic_cast<const DataTypeInt16 	*>(&argument_type))	return new AggregateFunctionTemplate<Int16>;
	else if (dynamic_cast<const DataTypeInt32 	*>(&argument_type))	return new AggregateFunctionTemplate<Int32>;
	else if (dynamic_cast<const DataTypeInt64 	*>(&argument_type))	return new AggregateFunctionTemplate<Int64>;
	else if (dynamic_cast<const DataTypeFloat32 *>(&argument_type))	return new AggregateFunctionTemplate<Float32>;
	else if (dynamic_cast<const DataTypeFloat64 *>(&argument_type))	return new AggregateFunctionTemplate<Float64>;
	else
		return NULL;
}

/** Создать агрегатную функцию с числовым типом в параметре шаблона, в зависимости от имени типа, расположенном в type_id.
  */
template<template <typename> class AggregateFunctionTemplate>
static IAggregateFunction * createWithNumericType(const String & type_id, size_t prefix_length)
{
		 if (0 == type_id.compare(prefix_length, strlen("UInt8"), 	"UInt8"))	return new AggregateFunctionTemplate<UInt8>;
	else if (0 == type_id.compare(prefix_length, strlen("UInt16"), 	"UInt16"))	return new AggregateFunctionTemplate<UInt16>;
	else if (0 == type_id.compare(prefix_length, strlen("UInt32"), 	"UInt32"))	return new AggregateFunctionTemplate<UInt32>;
	else if (0 == type_id.compare(prefix_length, strlen("UInt64"), 	"UInt64"))	return new AggregateFunctionTemplate<UInt64>;
	else if (0 == type_id.compare(prefix_length, strlen("Int8"), 	"Int8"))	return new AggregateFunctionTemplate<Int8>;
	else if (0 == type_id.compare(prefix_length, strlen("Int16"), 	"Int16"))	return new AggregateFunctionTemplate<Int16>;
	else if (0 == type_id.compare(prefix_length, strlen("Int32"), 	"Int32"))	return new AggregateFunctionTemplate<Int32>;
	else if (0 == type_id.compare(prefix_length, strlen("Int64"), 	"Int64"))	return new AggregateFunctionTemplate<Int64>;
	else if (0 == type_id.compare(prefix_length, strlen("Float32"),	"Float32"))	return new AggregateFunctionTemplate<Float32>;
	else if (0 == type_id.compare(prefix_length, strlen("Float64"),	"Float64"))	return new AggregateFunctionTemplate<Float64>;
	else
		return NULL;
}


AggregateFunctionPtr AggregateFunctionFactory::get(const String & name, const DataTypes & argument_types) const
{
	if (name == "count")
		return new AggregateFunctionCount;
	else if (name == "any")
		return new AggregateFunctionAny;
	else if (name == "anyLast")
		return new AggregateFunctionAnyLast;
	else if (name == "min")
		return new AggregateFunctionMin;
	else if (name == "max")
		return new AggregateFunctionMax;
	else if (name == "groupArray")
		return new AggregateFunctionGroupArray;
	else if (name == "sum")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionSum>(*argument_types[0]);

		if (!res)
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

		return res;
	}
	else if (name == "sumIf")
	{
		if (argument_types.size() != 2)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionSumIf>(*argument_types[0]);

		if (!res)
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

		return res;
	}
	else if (name == "avg")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionAvg>(*argument_types[0]);

		if (!res)
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

		return res;
	}
	else if (name == "avgIf")
	{
		if (argument_types.size() != 2)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionAvgIf>(*argument_types[0]);

		if (!res)
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

		return res;
	}
	else if (name == "uniq")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		const IDataType & argument_type = *argument_types[0];

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionUniq>(*argument_types[0]);

		if (res)
			return res;
		else if (dynamic_cast<const DataTypeDate 	*>(&argument_type))
			return new AggregateFunctionUniq<DataTypeDate::FieldType>;
		else if (dynamic_cast<const DataTypeDateTime*>(&argument_type))
			return new AggregateFunctionUniq<DataTypeDateTime::FieldType>;
		else if (dynamic_cast<const DataTypeString*>(&argument_type) || dynamic_cast<const DataTypeFixedString*>(&argument_type))
			return new AggregateFunctionUniq<String>;
		else
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);
	}
	else if (name == "uniqIf")
	{
		if (argument_types.size() != 2)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		const IDataType & argument_type = *argument_types[0];

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionUniqIf>(*argument_types[0]);

		if (res)
			return res;
		else if (dynamic_cast<const DataTypeDate 	*>(&argument_type))
			return new AggregateFunctionUniqIf<DataTypeDate::FieldType>;
		else if (dynamic_cast<const DataTypeDateTime*>(&argument_type))
			return new AggregateFunctionUniqIf<DataTypeDateTime::FieldType>;
		else if (dynamic_cast<const DataTypeString*>(&argument_type) || dynamic_cast<const DataTypeFixedString*>(&argument_type))
			return new AggregateFunctionUniqIf<String>;
		else
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);
	}
	else if (name == "uniqState")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		const IDataType & argument_type = *argument_types[0];

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionUniqState>(*argument_types[0]);

		if (res)
			return res;
		else if (dynamic_cast<const DataTypeDate 	*>(&argument_type))
			return new AggregateFunctionUniqState<DataTypeDate::FieldType>;
		else if (dynamic_cast<const DataTypeDateTime*>(&argument_type))
			return new AggregateFunctionUniqState<DataTypeDateTime::FieldType>;
		else if (dynamic_cast<const DataTypeString*>(&argument_type) || dynamic_cast<const DataTypeFixedString*>(&argument_type))
			return new AggregateFunctionUniqState<String>;
		else
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);
	}
	else if (name == "median" || name == "quantile")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);
		
		const IDataType & argument_type = *argument_types[0];

			 if (dynamic_cast<const DataTypeUInt8 	*>(&argument_type))	return new AggregateFunctionQuantile<UInt8>;
		else if (dynamic_cast<const DataTypeUInt16 	*>(&argument_type))	return new AggregateFunctionQuantile<UInt16>;
		else if (dynamic_cast<const DataTypeUInt32 	*>(&argument_type))	return new AggregateFunctionQuantile<UInt32>;
		else if (dynamic_cast<const DataTypeUInt64 	*>(&argument_type))	return new AggregateFunctionQuantile<UInt64>;
		else if (dynamic_cast<const DataTypeInt8 	*>(&argument_type))	return new AggregateFunctionQuantile<Int8>;
		else if (dynamic_cast<const DataTypeInt16 	*>(&argument_type))	return new AggregateFunctionQuantile<Int16>;
		else if (dynamic_cast<const DataTypeInt32 	*>(&argument_type))	return new AggregateFunctionQuantile<Int32>;
		else if (dynamic_cast<const DataTypeInt64 	*>(&argument_type))	return new AggregateFunctionQuantile<Int64>;
		else if (dynamic_cast<const DataTypeFloat32 *>(&argument_type))	return new AggregateFunctionQuantile<Float32>;
		else if (dynamic_cast<const DataTypeFloat64 *>(&argument_type))	return new AggregateFunctionQuantile<Float64>;
		else if (dynamic_cast<const DataTypeDate 	*>(&argument_type)) return new AggregateFunctionQuantile<DataTypeDate::FieldType, false>;
		else if (dynamic_cast<const DataTypeDateTime*>(&argument_type)) return new AggregateFunctionQuantile<DataTypeDateTime::FieldType, false>;
		else
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);
	}
	else if (name == "quantiles")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		const IDataType & argument_type = *argument_types[0];

			 if (dynamic_cast<const DataTypeUInt8 	*>(&argument_type))	return new AggregateFunctionQuantiles<UInt8>;
		else if (dynamic_cast<const DataTypeUInt16 	*>(&argument_type))	return new AggregateFunctionQuantiles<UInt16>;
		else if (dynamic_cast<const DataTypeUInt32 	*>(&argument_type))	return new AggregateFunctionQuantiles<UInt32>;
		else if (dynamic_cast<const DataTypeUInt64 	*>(&argument_type))	return new AggregateFunctionQuantiles<UInt64>;
		else if (dynamic_cast<const DataTypeInt8 	*>(&argument_type))	return new AggregateFunctionQuantiles<Int8>;
		else if (dynamic_cast<const DataTypeInt16 	*>(&argument_type))	return new AggregateFunctionQuantiles<Int16>;
		else if (dynamic_cast<const DataTypeInt32 	*>(&argument_type))	return new AggregateFunctionQuantiles<Int32>;
		else if (dynamic_cast<const DataTypeInt64 	*>(&argument_type))	return new AggregateFunctionQuantiles<Int64>;
		else if (dynamic_cast<const DataTypeFloat32 *>(&argument_type))	return new AggregateFunctionQuantiles<Float32>;
		else if (dynamic_cast<const DataTypeFloat64 *>(&argument_type))	return new AggregateFunctionQuantiles<Float64>;
		else if (dynamic_cast<const DataTypeDate 	*>(&argument_type)) return new AggregateFunctionQuantiles<DataTypeDate::FieldType, false>;
		else if (dynamic_cast<const DataTypeDateTime*>(&argument_type)) return new AggregateFunctionQuantiles<DataTypeDateTime::FieldType, false>;
		else
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);
	}
	else if (name == "medianTiming" || name == "quantileTiming")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionQuantileTiming>(*argument_types[0]);

		if (!res)
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

		return res;
	}
	else if (name == "quantilesTiming")
	{
		if (argument_types.size() != 1)
			throw Exception("Incorrect number of arguments for aggregate function " + name, ErrorCodes::NUMBER_OF_ARGUMENTS_DOESNT_MATCH);

		AggregateFunctionPtr res = createWithNumericType<AggregateFunctionQuantilesTiming>(*argument_types[0]);

		if (!res)
			throw Exception("Illegal type " + argument_types[0]->getName() + " of argument for aggregate function " + name, ErrorCodes::ILLEGAL_TYPE_OF_ARGUMENT);

		return res;
	}
	else
		throw Exception("Unknown aggregate function " + name, ErrorCodes::UNKNOWN_AGGREGATE_FUNCTION);
}


AggregateFunctionPtr AggregateFunctionFactory::tryGet(const String & name, const DataTypes & argument_types) const
{
	return isAggregateFunctionName(name)
		? get(name, argument_types)
		: NULL;
}


bool AggregateFunctionFactory::isAggregateFunctionName(const String & name) const
{
	static const char * names[] =
	{
		"count",
		"any",
		"anyLast",
		"min",
		"max",
		"sum",
		"sumIf",
		"avg",
		"avgIf",
		"uniq",
		"uniqIf",
		"uniqState",
		"groupArray",
		"median",
		"quantile",
		"quantiles",
		"medianTiming",
		"quantileTiming",
		"quantilesTiming",
		NULL
	};

	for (const char ** it = names; *it; ++it)
		if (0 == strcmp(*it, name.data()))
			return true;

	return false;
}


}
