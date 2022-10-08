#pragma once

#include <vector>

#include <arrow/result.h>

#include "DataRow.h"


arrow::Status writeRowsToFile(
      const std::string& filename,
      const std::vector<DataRow>& rows
);
