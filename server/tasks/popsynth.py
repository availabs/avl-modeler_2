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
# import psycopg2


queue = rq.Queue('rq_popsynth', connection=Redis())


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

        con = sqlite3.connect("database/activitysimserver.sqlite")
        status = "failed"
        cur = con.cursor()
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

    con = sqlite3.connect("database/activitysimserver.sqlite")

    df_datadic.to_sql("project_"+project_id+"_datadict", con)
    df_persons.to_sql("project_"+project_id+"_persons", con)
    df_households.to_sql("project_"+project_id+"_households", con)
    df_geocrosswalk.to_sql("project_"+project_id+"_geocrosswalk", con)

    # update status in project table
    status = "complete"
    cur = con.cursor()
    cur.execute('''UPDATE projects SET status = ? WHERE id = ?''',
                (status, project_id))

    print("project Table's status: ")
    data = cur.execute(
        '''SELECT status FROM projects where id = ?''', (project_id,))
    for row in data:
        print("project status of", project_id, "is", row)

    # conn.execute("CREATE TABLE students"+project_id+'datadict"(name TEXT, addr TEXT, city TEXT, pin TEXT)')

#  create landuse table in sqldb

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

#  create distance matrix table in sqldb

    # selectedBGsStr = ''.join(selectedBGs)
    selectedBGsStr ="'" + "','".join(selectedBGs) + "'"
    print("selectedBGsStr",selectedBGs, selectedBGsStr)

    cur.execute(''' SELECT load_extension('/usr/local/lib/mod_spatialite')''')
    
    cur.execute(''' CREATE TABLE project_'''+project_id+'''_distance (
                        --ogc_fid	INTEGER PRIMARY KEY AUTOINCREMENT,
                            geoid_1 TEXT NOT NULL,
                            geoid_2 TEXT NOT NULL,
                            distance FLOAT NOT NULL
                        )
                    ''')

    cur.execute(''' INSERT INTO project_'''+project_id+'''_distance(geoid_1, geoid_2, distance)
                        SELECT a.geoid,
                            b.geoid,
                            ST_Distance(ST_Centroid(GeomFromWKB(a.geometry)), ST_Centroid(GeomFromWKB(b.geometry))) AS distance
                        FROM tl_2019_36_bg AS a
                        CROSS JOIN (
                            SELECT geoid, geometry
                            FROM tl_2019_36_bg
                            WHERE  a.geoid in ('''+selectedBGsStr+''')
                            ORDER BY geoid
                      
                        ) AS b
                        WHERE  a.geoid in ('''+selectedBGsStr+''')
                        ORDER BY a.geoid, b.geoid
                       

                    ''')


    con.commit()
    con.close()

    # delete local folder

    shutil.rmtree(folder)

    print('PopSynth run successfully',
          " and local task folder", project_id, "deleted")
