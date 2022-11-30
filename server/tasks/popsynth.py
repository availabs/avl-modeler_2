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



queue = rq.Queue('rq_popsynth', connection=Redis()) 

# def run_popsynth(args,project_id):
#     print('Starting task')
#     print('task args test', args)
#     run(args)
#     print('Task completed')
#     job = queue.enqueue('tasks.popsynth.load_popsynth_run',args.working_dir, project_id)  

#     return "PopSynth run completed"


def run_popsynth(args,project_id):
    print('Starting task')
    print('task args test', args)
    try:
        run(args)
        job = queue.enqueue('tasks.popsynth.load_popsynth_run',args.working_dir, project_id)
        print('Task completed')
    except: 
      # this code currently never runs because run function doesn't raise exception on failure
      # insert failed project status into database

      con = sqlite3.connect("database/activitysimserver.sqlite")
      status="failed"
      cur = con.cursor()
      cur.execute('''UPDATE projects SET status = ? WHERE id = ?''', (status, project_id))

      con.commit()
      con.close()
      
      print('PopSynth run failed')

    #   return "failed status inserted"
      




def load_popsynth_run(folder, project_id):
    print('load popsynth from folder', folder)
    
    ## update the projects status to loading
    ## create table in sqlite project_%id_datadict
    ## create table in sqlite project_%id_persons
    ## create table in sqlite project_%id_households
    ## open those files and write them into the corresponding tables
    ## update the status of the project to 'complete'

    # load data
    print ("path", folder + '/output')
    
    df_datadic = pd.read_csv(folder +'/output/data_dict.csv', sep=',',
                    error_bad_lines=False, index_col=False, dtype='unicode')
                    
    df_persons = pd.read_csv(folder +'/output/synthetic_persons.csv', sep=',',
                    error_bad_lines=False, index_col=False, dtype='unicode')

    df_households = pd.read_csv(folder +'/output/synthetic_households.csv', sep=',',
                    error_bad_lines=False, index_col=False, dtype='unicode')

    df_geocrosswalk = pd.read_csv(folder +'/data/geo_cross_walk.csv', sep=',',
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


    #update status in project table
    status="complete"
    cur = con.cursor()
    cur.execute('''UPDATE projects SET status = ? WHERE id = ?''', (status, project_id))

    print("project Table's status: ")
    data = cur.execute('''SELECT status FROM projects where id = ?''',(project_id,))
    for row in data:
        print("project status of", project_id,"is", row)

    con.commit()
    con.close()



    ## delete task folder

    # shutil.rmtree(folder)

    print('PopSynth run successfully', " and local task folder", project_id, "deleted")

    
  



   