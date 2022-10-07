#pragma once

#include <arrow/api.h>
#include <arrow/result.h>

template<class T>
arrow::Result<std::shared_ptr<arrow::Table>>
VectorToColumnarTable(const std::vector<T>& rows)
{
   // The builders are more efficient using
   // arrow::jemalloc::MemoryPool::default_pool() as this can increase the size of
   // the underlying memory regions in-place. At the moment, arrow::jemalloc is only
   // supported on Unix systems, not Windows.
   arrow::MemoryPool* pool = arrow::default_memory_pool();

   arrow::Int64Builder id_builder(pool);
   arrow::Int64Builder components_builder(pool);
   arrow::ListBuilder component_cost_builder(
         pool,
         std::make_shared<arrow::DoubleBuilder>(pool)
   );
   // The following builder is owned by component_cost_builder.
   auto* component_item_cost_builder = dynamic_cast<arrow::DoubleBuilder*>(
         component_cost_builder.value_builder()
   );

   // Now we can loop over our existing data and insert it into the builders. The
   // `Append` calls here may fail (e.g. we cannot allocate enough additional memory).
   // Thus we need to check their return values. For more information on these values,
   // check the documentation about `arrow::Status`.
   for (const T& row: rows)
   {
      ARROW_RETURN_NOT_OK(id_builder.Append(row.id));
      ARROW_RETURN_NOT_OK(components_builder.Append(row.components));

      // Indicate the start of a new list row. This will memorise the current
      // offset in the values builder.
      ARROW_RETURN_NOT_OK(component_cost_builder.Append());
      // Store the actual values. The same memory layout is
      // used for the component cost data, in this case a vector of
      // type double, as for the memory that Arrow uses to hold this
      // data and will be created.
      ARROW_RETURN_NOT_OK(component_item_cost_builder->AppendValues(
            row.component_cost.data(), row.component_cost.size()));
   }

   // At the end, we finalise the arrays, declare the (type) schema and combine them
   // into a single `arrow::Table`:
   std::shared_ptr<arrow::Array> id_array;
   ARROW_RETURN_NOT_OK(id_builder.Finish(&id_array));
   std::shared_ptr<arrow::Array> components_array;
   ARROW_RETURN_NOT_OK(components_builder.Finish(&components_array));
   // No need to invoke component_item_cost_builder.Finish because it is implied by
   // the parent builder's Finish invocation.
   std::shared_ptr<arrow::Array> component_cost_array;
   ARROW_RETURN_NOT_OK(component_cost_builder.Finish(&component_cost_array));

   std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
         arrow::field("id", arrow::int64()),
         arrow::field("components", arrow::int64()),
         arrow::field("component_cost", arrow::list(arrow::float64()))
   };

   auto schema = std::make_shared<arrow::Schema>(schema_vector);

   // The final `table` variable is the one we can then pass on to other functions
   // that can consume Apache Arrow memory structures. This object has ownership of
   // all referenced data, thus we don't have to care about undefined references once
   // we leave the scope of the function building the table and its underlying arrays.
   std::shared_ptr<arrow::Table> table =
         arrow::Table::Make(schema, {id_array, components_array, component_cost_array});

   return table;
}


template<class T>
arrow::Status writeRowsToFile(const std::string& filename, const std::vector<T>& rows)
{
   std::shared_ptr<arrow::Table> table;

   ARROW_ASSIGN_OR_RAISE(table, VectorToColumnarTable(rows));


   return arrow::Status::OK();
}
