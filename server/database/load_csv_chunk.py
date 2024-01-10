import sqlite3
import pandas as pd
import dask.dataframe as dd


# load data by chuck

chunksize = 1000
# for chunk in pd.read_csv('./data/psam_p36.csv', sep=',',
#                   index_col=False, dtype='unicode', on_bad_lines='skip', chunksize=chunksize):

for chunk in dd.read_csv('./data/psam_p36.csv', sep=',',
        chunksize=chunksize):

# strip whitespace from headers
    chunk.columns = chunk.columns.str.strip()


    con = sqlite3.connect("./activitysimserver.sqlite")

# drop data into database
    chunk.to_sql("psam_p36", con,  if_exists="append")


con.close()
