#include "ArrowUtilities.h"

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>


arrow::Result<std::shared_ptr<arrow::Table>> vectorToColumnarTable(
      const std::vector<DataRow>& rows
)
{
   // Adapted from https://arrow.apache.org/docs/cpp/examples/row_columnar_conversion.html

   arrow::MemoryPool* pool = arrow::default_memory_pool();

   arrow::DoubleBuilder errorBuilder(pool);
   arrow::ListBuilder component_cost_builder(
         pool,
         std::make_shared<arrow::DoubleBuilder>(pool)
   );

   // Now we can loop over our existing data and insert it into the builders. The
   // `Append` calls here may fail (e.g. we cannot allocate enough additional memory).
   // Thus, we need to check their return values. For more information on these values,
   // check the documentation about `arrow::Status`.
   for (const auto& row: rows)
   {
      ARROW_RETURN_NOT_OK(errorBuilder.Append(row.squaredError));
   }

   // At the end, we finalise the arrays, declare the (type) schema and combine them
   // into a single `arrow::Table`:
   std::shared_ptr<arrow::Array> errorArray;
   ARROW_RETURN_NOT_OK(errorBuilder.Finish(&errorArray));

   std::vector<std::shared_ptr<arrow::Field>> schemaVector = {
         arrow::field("SquaredError", arrow::float64()),
   };

   auto schema = std::make_shared<arrow::Schema>(schemaVector);

   // The final `table` variable is the one we can then pass on to other functions
   // that can consume Apache Arrow memory structures. This object has ownership of
   // all referenced data, thus we don't have to care about undefined references once
   // we leave the scope of the function building the table and its underlying arrays.
   std::shared_ptr<arrow::Table> table = arrow::Table::Make(schema, {errorArray,});

   return table;
}


arrow::Status writeRowsToFile(
      const std::string& filename,
      const std::vector<DataRow>& rows
)
{
   // https://github.com/apache/arrow/blob/master/cpp/examples/tutorial_examples/file_access_example.cc#L69
   std::shared_ptr<arrow::Table> table;

   ARROW_ASSIGN_OR_RAISE(table, vectorToColumnarTable(rows))

   std::shared_ptr<arrow::io::FileOutputStream> file;
   ARROW_ASSIGN_OR_RAISE(file, arrow::io::FileOutputStream::Open(filename, false))
   ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::ipc::RecordBatchWriter> ipc_writer,
                         arrow::ipc::MakeFileWriter(file, table->schema()))
   ARROW_RETURN_NOT_OK(ipc_writer->WriteTable(*table));
   ARROW_RETURN_NOT_OK(ipc_writer->Close());

   return arrow::Status::OK();
}
