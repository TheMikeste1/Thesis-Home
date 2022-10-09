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

   arrow::StringBuilder idBuilder(pool);
   arrow::StringBuilder distributionBuilder(pool);
   arrow::StringBuilder votingMechanismBuilder(pool);
   arrow::Int32Builder numberOfProxiesBuilder(pool);
   arrow::Int32Builder numberOfInactivesBuilder(pool);
   arrow::DoubleBuilder estimateBuilder(pool);

   // Now we can loop over our existing data and insert it into the builders. The
   // `Append` calls here may fail (e.g. we cannot allocate enough additional memory).
   // Thus, we need to check their return values. For more information on these values,
   // check the documentation about `arrow::Status`.
   for (const auto& row: rows)
   {
      ARROW_RETURN_NOT_OK(idBuilder.Append(row.id));
      ARROW_RETURN_NOT_OK(distributionBuilder.Append(row.distribution));
      ARROW_RETURN_NOT_OK(votingMechanismBuilder.Append(row.votingMechanism));
      ARROW_RETURN_NOT_OK(numberOfProxiesBuilder.Append(row.numberOfProxies));
      ARROW_RETURN_NOT_OK(numberOfInactivesBuilder.Append(row.numberOfInactives));
      ARROW_RETURN_NOT_OK(estimateBuilder.Append(row.estimate));
   }

   // At the end, we finalise the arrays, declare the (type) schema and combine them
   // into a single `arrow::Table`:
   std::shared_ptr<arrow::Array> idArray;
   std::shared_ptr<arrow::Array> distributionArray;
   std::shared_ptr<arrow::Array> votingMechanismArray;
   std::shared_ptr<arrow::Array> numberOfProxiesArray;
   std::shared_ptr<arrow::Array> numberOfInactivesArray;
   std::shared_ptr<arrow::Array> estimatesArray;
   ARROW_RETURN_NOT_OK(estimateBuilder.Finish(&idArray));
   ARROW_RETURN_NOT_OK(estimateBuilder.Finish(&distributionArray));
   ARROW_RETURN_NOT_OK(estimateBuilder.Finish(&votingMechanismArray));
   ARROW_RETURN_NOT_OK(estimateBuilder.Finish(&numberOfProxiesArray));
   ARROW_RETURN_NOT_OK(estimateBuilder.Finish(&numberOfInactivesArray));
   ARROW_RETURN_NOT_OK(estimateBuilder.Finish(&estimatesArray));

   std::vector<std::shared_ptr<arrow::Field>> schemaVector = {
         arrow::field("id", arrow::utf8()),
         arrow::field("Distribution", arrow::utf8()),
         arrow::field("Voting Mechanism", arrow::utf8()),
         arrow::field("Number of Proxies", arrow::int32()),
         arrow::field("Number of Inactives", arrow::int32()),
         arrow::field("Estimate", arrow::float64()),
   };

   auto schema = std::make_shared<arrow::Schema>(schemaVector);

   // The final `table` variable is the one we can then pass on to other functions
   // that can consume Apache Arrow memory structures. This object has ownership of
   // all referenced data, thus we don't have to care about undefined references once
   // we leave the scope of the function building the table and its underlying arrays.
   std::shared_ptr<arrow::Table> table = arrow::Table::Make(
         schema,
         {
               idArray,
               distributionArray,
               votingMechanismArray,
               numberOfProxiesArray,
               numberOfInactivesArray,
               estimatesArray,
         }
   );

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
