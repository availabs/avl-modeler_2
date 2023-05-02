#from sim_settings.settings import models
#from activitysim.core import pipeline
from activitysim.cli.run import run
from populationsim import steps
from redis import Redis
from rq import Queue
import rq
import sqlite3
from sqlite3 import Error
import pandas as pd
import shutil

import openmatrix as omx
import numpy as np

import pandas as pd



# import psycopg2


queue = rq.Queue('rq_popsynth', connection=Redis())

def get_db_connection():
    conn = sqlite3.connect('database/activitysimserver.sqlite')
    conn.row_factory = row_to_dict
    return conn

def row_to_dict(cursor: sqlite3.Cursor, row: sqlite3.Row) -> dict:
    data = {}
    for idx, col in enumerate(cursor.description):
        data[col[0]] = row[idx]
    return data

# conpostgres = psycopg2.connect(host='mars.availabs.org', database='height',
#     user='postgres', password='Jedi21funk')
# curpostgres = conpostgres.cursor()


# def run_popsynth(args,project_id):
#     print('Starting task')
#     print('task args test', args)
#     run(args)
#     print('Task completed')
#     job = queue.enqueue('tasks.popsynth.load_popsynth_run',args.working_dir, project_id)

#     return "PopSynth run completed"


def run_popsynth(args, project_id, selectedBGs):
    print('Starting task')
    print('task args test', args)
    try:
        run(args)
        job = queue.enqueue('tasks.popsynth.load_popsynth_run',
                            args.working_dir, project_id, selectedBGs)
        print('Task completed')
    except:
        # this code currently never runs because run function doesn't raise exception on failure
        # insert failed project status into database

        con = get_db_connection()
        cur = con.cursor()
        status = "failed"
        # cur = con.cursor()
        cur.execute('''UPDATE projects SET status = ? WHERE id = ?''',
                    (status, project_id))

        con.commit()
        con.close()

        print('PopSynth run failed')

    #   return "failed status inserted"


def load_popsynth_run(folder, project_id, selectedBGs):
    # def load_popsynth_run(folder, project_id, args):
    print('load popsynth from folder', folder)

    # update the projects status to loading
    # create table in sqlite project_%id_datadict
    # create table in sqlite project_%id_persons
    # create table in sqlite project_%id_households
    # open those files and write them into the corresponding tables
    # update the status of the project to 'complete'

    # load data
    print("path", folder + '/output')

    df_datadic = pd.read_csv(folder + '/output/data_dict.csv', sep=',',
                             error_bad_lines=False, index_col=False, dtype='unicode')

    df_persons = pd.read_csv(folder + '/output/synthetic_persons.csv', sep=',',
                             error_bad_lines=False, index_col=False, dtype='unicode')

    df_households = pd.read_csv(folder + '/output/synthetic_households.csv', sep=',',
                                error_bad_lines=False, index_col=False, dtype='unicode')

    df_geocrosswalk = pd.read_csv(folder + '/data/geo_cross_walk.csv', sep=',',
                                  error_bad_lines=False, index_col=False, dtype='unicode')

    # strip whitespace from headers
    df_datadic.columns = df_datadic.columns.str.strip()
    df_persons.columns = df_persons.columns.str.strip()
    df_households.columns = df_households.columns.str.strip()
    df_geocrosswalk.columns = df_geocrosswalk.columns.str.strip()

    # drop data into database
    # df.to_sql("project_datadict", con)

    con = get_db_connection()
    cur = con.cursor()

    df_datadic.to_sql("project_"+project_id+"_datadict", con)
    df_persons.to_sql("project_"+project_id+"_persons", con)
    df_households.to_sql("project_"+project_id+"_households", con)
    df_geocrosswalk.to_sql("project_"+project_id+"_geocrosswalk", con)

    # update status in project table
    status = "complete"
    # cur = con.cursor()
    cur.execute('''UPDATE projects SET status = ? WHERE id = ?''',
                (status, project_id))

    print("project Table's status: ")
    data = cur.execute(
        '''SELECT status FROM projects where id = ?''', (project_id,))
    for row in data:
        print("project status of", project_id, "is", row)

    # conn.execute("CREATE TABLE students"+project_id+'datadict"(name TEXT, addr TEXT, city TEXT, pin TEXT)')

    con.commit()
    con.close()

    # delete local folder

    # shutil.rmtree(folder)

    #call create_landuse
    create_landuse_table(project_id, selectedBGs)

    return "{\"reponse\": \"success inserting datadict,person,households,geocrosswalk\"}"


#  create landuse table in sqldb


def create_landuse_table(project_id, selectedBGs):
    con = get_db_connection()
    cur = con.cursor()

    cur.execute('''CREATE TABLE project_'''+project_id+'''_landuse AS
                    select 
                    tract || bg as TAZ, 
                    tract as DISTRICT,
                    count( distinct household_id) as TOTHH,
                    count(1) as TOTPOP,
                    100 as TOTACRE, -- will need to join on spatial data
                    70 as RESARCRE, 
                    30 as CIACRE,
                    sum(CASE  when ESR is null then 0 else 1 end) as TOTEMP,
                    sum(CASE  when ( CAST(AGEP AS INTEGER) < 19 and CAST(AGEP AS INTEGER) >= 5 ) then 1 else 0 end) as AGE519,
                    sum(CASE  when CAST(ESR AS INTEGER) = 1.0  then 1 else 0 end) as RETEMP,
                    sum(CASE  when CAST(ESR AS INTEGER) = 2.0  then 1 else 0 end) as FPSEMP,
                    sum(CASE  when CAST(ESR AS INTEGER) = 3.0 then 1 else 0 end) as HEREMP,
                    sum(CASE  when CAST(ESR AS INTEGER) = 4.0  then 1 else 0 end) as AGREMP,
                    sum(CASE  when CAST(ESR AS INTEGER) = 5.0  then 1 else 0 end) as MWTEMP,
                    sum(CASE  when CAST(ESR AS INTEGER) = 6.0  then 1 else 0 end) as OTHEMP,
                    5 as PRKCST,
                    10 as OPRKCST,
                    0 as area_type,
                    sum(CASE  when (CAST(SCHG AS INTEGER) < 15 and CAST(SCHG AS INTEGER) >= 11) then 1 else 0 end) as HSENROLL,
                    sum(CASE  when ( CAST(SCHG AS INTEGER) < 15 and CAST(SCHG AS INTEGER) >= 11 and CAST(WKHP AS INTEGER) >=30) then 1 else 0 end) as COLLFTE,
                    sum(CASE  when ( CAST(SCHG AS INTEGER) < 15 and CAST(SCHG AS INTEGER) >= 11 and CAST(WKHP AS INTEGER) < 30) then 1 else 0 end) as COLLPTE,
                    JWMNP as TERMINAL

                    from project_'''+project_id+'''_persons
                    group by taz

                    ''')
    con.commit()
    con.close()

     
    # create_distance_table(project_id, selectedBGs)
    matrix_omx(project_id,selectedBGs)

    return "{\"reponse\": \"success inserting landuse\"}"



# def create_distance_table(project_id, selectedBGs):

#     con = get_db_connection()
#     cur = con.cursor()
# #  create distance matrix table in sqldb

#     # selectedBGsStr = ''.join(selectedBGs)
#     selectedBGsStr ="'" + "','".join(selectedBGs) + "'"
#     print("selectedBGs",selectedBGs, len(selectedBGs))
#     print("selectedBGsStr", selectedBGsStr,  len(selectedBGsStr))

# #   important to enable other wise permission error of load_extention
#     con.enable_load_extension(True)

#     cur.execute(''' SELECT load_extension('/usr/local/lib/mod_spatialite')''')
#     # cur.execute(''' SELECT load_extension('/home/jin/miniconda3/envs/ASIM_DEV/lib/mod_spatialite')''')
#     # cur.execute(''' SELECT load_extension('mod_spatialite')''')
    
#     cur.execute(''' CREATE TABLE project_'''+project_id+'''_distance (
#                         --ogc_fid	INTEGER PRIMARY KEY AUTOINCREMENT,
#                             geoid_1 TEXT NOT NULL,
#                             geoid_2 TEXT NOT NULL,
#                             distance FLOAT NOT NULL
#                         )
#                     ''')

#     cur.execute(''' INSERT INTO project_'''+project_id+'''_distance(geoid_1, geoid_2, distance)
#                     SELECT a.geoid as geoid_1,
#                         b.geoid as geoid_2,
#                         ST_Distance(ST_Centroid(GeomFromWKB(a.geometry)), ST_Centroid(GeomFromWKB(b.geometry))) AS distance
#                     FROM tl_2019_36_bg AS a
#                     CROSS JOIN (
#                         SELECT geoid, geometry
#                         FROM tl_2019_36_bg
                   
#                         WHERE  geoid in ('''+selectedBGsStr+''')

#                         ORDER BY geoid

#                     ) AS b
#                     WHERE a.geoid in ('''+selectedBGsStr+''')
#                     ORDER BY a.geoid, b.geoid
                    
#                     ''')


#     con.commit()
#     con.close()

#     # delete local folder
#     # shutil.rmtree(folder)


#     print('PopSynth run successfully',
#           " and local task folder", project_id, "deleted")
  
#     #call matrix_omx()
#     matrix_omx(selectedBGs)


def matrix_omx(project_id,selectedBGs):
    
    con = get_db_connection()
    cur = con.cursor()

    # Create some data
    bg_counts =   len(selectedBGs)
    ones = np.ones((bg_counts,bg_counts))
    twos = 2.0*ones

    # selectedBGsStr = ''.join(selectedBGs)
    selectedBGsStr ="'" + "','".join(selectedBGs) + "'"
    # print("selectedBGs",selectedBGs, len(selectedBGs))
    # print("selectedBGsStr", selectedBGsStr,  len(selectedBGsStr))

# load specialite
    con.enable_load_extension(True)

    cur.execute(''' SELECT load_extension('/usr/local/lib/mod_spatialite')''')

    distancesql = f'''SELECT a.geoid as geoid_1,
                        b.geoid as geoid_2,
                        ST_Distance(ST_Centroid(GeomFromWKB(a.geometry)), ST_Centroid(GeomFromWKB(b.geometry))) AS distance
                    FROM tl_2019_36_bg AS a
                    CROSS JOIN (
                        SELECT geoid, geometry
                        FROM tl_2019_36_bg
                   
                        WHERE  geoid in ('''+selectedBGsStr+''')

                        ORDER BY geoid

                    ) AS b
                    WHERE a.geoid in ('''+selectedBGsStr+''')
                    ORDER BY a.geoid, b.geoid
                    '''
    distance_output = con.execute(distancesql).fetchall()

    print("distance_output----------", distance_output[:10], ones[:10] )

# trying some matrix test

# input array
    # distance_output = [{'geoid_1': '360359701001', 'geoid_2': '360359701001', 'distance': 0.0},
    #     {'geoid_1': '360359701001', 'geoid_2': '360359701002', 'distance': 0.06108229323259779}]

    # Extract unique geoid_1 and geoid_2
    geoid_1 = np.unique([d['geoid_1'] for d in distance_output])
    geoid_2 = np.unique([d['geoid_2'] for d in distance_output])

    # Create new array filled with ones
    distancesTable = np.ones((len(geoid_1), len(geoid_2)))

    # Fill in distances from input array
    for d in distance_output:
        i = np.where(geoid_1 == d['geoid_1'])[0][0]
        j = np.where(geoid_2 == d['geoid_2'])[0][0]
        distancesTable[i, j] = d['distance']

    # Print array
    print("distancesTable-------", distancesTable)


    # Create an OMX file (will overwrite existing file!)
    print('Creating skim.omx')
    skim = omx.open_file('skim.omx','w')   # use 'a' to append/edit an existing file

    # Write to the file.
    skim['distance'] = ones
    skim['distance2'] = twos
    skim['distance3'] = ones + twos 
    skim['distance4'] = distancesTable        


    distance_data = skim['distance4'][:]

    print("skim['distance4']", skim['distance4'], distance_data)


# Create a pandas DataFrame with the new column and row names ( turns out no need)
    df = pd.DataFrame(distance_data, columns=geoid_1, index=geoid_1)

    print("DataFrame", df, df.dtypes)


    # skim['distance5'] = df 


    skim.close()







    # Open an OMX file for reading only
    print('Reading skim.omx')
    skim = omx.open_file('skim.omx')

    print ('Shape:', skim.shape())                 # (100,100)
    print ('Number of tables:', len(skim))         # 3
    print ('Table names:', skim.list_matrices())   # ['distance','distance2',',distance3']

    # Work with data. Pass a string to select matrix by name:
    # -------------------------------------------------------
    distance = skim['distance']
    distance2 = skim['distance2']
    distance3 = skim['distance3']

    # halves = distance * 0.5  # CRASH!  Don't modify an OMX object directly.
    #                    # Create a new numpy array, and then edit it.
    halves = np.array(distance) * 0.5

    first_row = distance2[0]
    first_row[:] = 0.5 * first_row[:]

    my_very_special_zone_value = distance2[10][25]

    print('halves:', halves) 

    print('my_very_special_zone_value:', my_very_special_zone_value) 




    # FANCY: Use attributes to find matrices
    # --------------------------------------
    skim.close()                            # was opened read-only, so let's reopen.
    skim = omx.open_file('skim.omx','a')  # append mode: read/write existing file

    skim['distance'].attrs.timeperiod = 'am'
    skim['distance'].attrs.mode = 'hwy'

    skim['distance2'].attrs.timeperiod = 'md'

    skim['distance3'].attrs.timeperiod = 'am'
    skim['distance3'].attrs.mode = 'trn'

    print('attributes:', skim.list_all_attributes())       # ['mode','timeperiod']

    # Use a DICT to select matrices via attributes:

    all_am_trips = skim[ {'timeperiod':'am'} ]                    # [distance,distance3]
    all_hwy_trips = skim[ {'mode':'hwy'} ]                        # [distance]
    all_am_trn_trips = skim[ {'mode':'trn','timeperiod':'am'} ]   # [distance3]

    print('sum of some tables:', np.sum(all_am_trips))

    # SUPER FANCY: Create a mapping to use TAZ numbers instead of matrix offsets
    # --------------------------------------------------------------------------
    # (any mapping would work, such as a mapping with large gaps between zone
    #  numbers. For this simple case we'll just assume TAZ numbers are 1-100.)

    taz_equivs = np.arange(1,bg_counts+1)                  # 1-100 inclusive

    skim.create_mapping('taz', taz_equivs)
    print('mappings:', skim.list_mappings()) # ['taz']

    tazs = skim.mapping('taz') # Returns a dict:  {1:0, 2:1, 3:2, ..., 100:99}
    distance3 = skim['distance3']
    print('cell value:', distance3[tazs[bg_counts]][tazs[bg_counts]]) # 3.0  (taz (100,100) is cell [99][99])
    print('all table 3', distance3)

    skim.close()

    # delete local folder

    # shutil.rmtree(folder)