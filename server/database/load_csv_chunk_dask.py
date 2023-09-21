import sqlite3
import dask.dataframe as dd

# Set up Dask dataframe and SQLite connection
filename = './data/psam_p36.csv'
database_path = "./activitysimserver.sqlite"
chunksize = 1000

# Read CSV file in chunks using Dask
ddf = dd.read_csv(filename, sep=',', dtype='unicode', on_bad_lines='skip', blocksize=None)

# Strip whitespace from column headers
ddf.columns = ddf.columns.str.strip()

# Establish SQLite connection
con = sqlite3.connect(database_path)

# Define function to write Dask dataframe chunk to SQLite
def write_chunk_to_sqlite(chunk):
    chunk.to_sql("psam_p36", con, if_exists="append", index=False)

# Map the write function to each chunk and persist to SQL
ddf.map_partitions(write_chunk_to_sqlite).compute(scheduler='processes')

# Close SQLite connection
con.close()
