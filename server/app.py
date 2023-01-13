import sqlite3
import string
import random
from flask import Flask, jsonify, render_template, request
from flask_cors import CORS, cross_origin
from markupsafe import re
#from sim_settings.settings import models
import argparse
import os
from activitysim.cli.run import add_run_args
import json
import csv
import yaml
import requests
from redis import Redis
from rq import Queue
import rq
# import popsynth


app = Flask(__name__)
cors = CORS(app, support_credentials=True)
app.config['CORS_HEADERS'] = 'Content-Type'


@app.route('/')
@cross_origin(supports_credentials=True)
def index():
    return 'Server Works!'


@app.route('/greet')
def say_hello():
    return 'Hello from Server'


def getHash():
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=6))


def get_db_connection():
    conn = sqlite3.connect('database/activitysimserver.sqlite')
    conn.row_factory = row_to_dict
    return conn


def row_to_dict(cursor: sqlite3.Cursor, row: sqlite3.Row) -> dict:
    data = {}
    for idx, col in enumerate(cursor.description):
        data[col[0]] = row[idx]
    return data


def formatBgId(bgs):
    # add statecode 36+ 3 digit county code + 7 digit blockgroupIDs?
    # if blockgroup id is 6 digit add preceding 0
    for i, bg in enumerate(bgs):
        bg[i] = bg.zfill(7)
    return bgs


@app.route('/projects/<userId>')
def projectsByUser(userId):
    conn = get_db_connection()
    # cursor = conn.cursor()
    projects = conn.execute(
        'SELECT a.id, a.Name as name, a.geoList as geo_list, a.options, a.status FROM projects_users INNER JOIN projects as a ON projects_users.project_id=a.id WHERE projects_users.user_id=?', (userId,)).fetchall()
    # print("what is user ID", userId, cursor.lastrowid)
    return jsonify(projects)


# status route to sequence the status
@app.route('/project/<projectId>/status')
def statusByProject(projectId):
    conn = get_db_connection()
    # cursor = conn.cursor()
    status = conn.execute(
        #     'SELECT a.id, a.status FROM projects as a where a.id =?', (projectId,)).fetchall()
        # return jsonify(status)

        'SELECT a.status FROM projects as a where a.id =?', (projectId,)).fetchall()
    return jsonify(status)


@app.route('/project/<projectId>/view')
def viewByProjectId(projectId):
    conn = get_db_connection()

    hhsql = "SELECT * FROM project_"+projectId+"_households"
    psql = "SELECT * FROM project_"+projectId+"_persons"

    households = conn.execute(
        hhsql).fetchall()
    persons = conn.execute(
        psql).fetchall()
    print("synpop households and persons",
          households[0].values(), persons[0].values())

    return jsonify({"projectID": projectId, "Households": households, "Persons": persons})
    # return jsonify({ "projectID": projectId,"totalHouseholds": households[0]["COUNT(1)"], "totalPersons": persons[0]["COUNT(1)"]})


@app.route('/project/<projectId>/list_vars')
def viewVarsByProjectId(projectId):
    conn = get_db_connection()

    hhsql = "SELECT name FROM PRAGMA_TABLE_INFO('project_" + \
        projectId+"_households')"
    psql = "SELECT name FROM PRAGMA_TABLE_INFO('project_" + \
        projectId+"_persons')"

    households = conn.execute(
        hhsql).fetchall()
    persons = conn.execute(
        psql).fetchall()

    hhkeys = ['h_' + d['name'] for d in households if 'name' in d][5:]
    personskeys = ['p_' + d['name'] for d in persons if 'name' in d][5:]

    # output
    # [
    #     hh_hinc,
    #     hh_wif,
    #     ...
    #     p_inc,
    #     p_age,
    # ]

    return jsonify(hhkeys+personskeys)


@app.route('/project/<projectId>/<selectedVar>')
def viewBySelectedVar(projectId, selectedVar):
    conn = get_db_connection()

    type = selectedVar[0]
    selectedVariable = selectedVar[2:]

    print("var_Names", selectedVar, type, selectedVariable)

    # rewrite to contain GEOID

    hhsql = f'''
	SELECT {selectedVariable},project_{projectId}_households.BG, project_{projectId}_geocrosswalk.BLKGRP as GEOID
    FROM project_{projectId}_households
    JOIN project_{projectId}_geocrosswalk ON PRINTF('%07d', project_{projectId}_households.BG) = project_{projectId}_geocrosswalk.BG'''

    psql = f'''SELECT {selectedVariable},project_{projectId}_persons.BG, project_{projectId}_geocrosswalk.BLKGRP as GEOID
    FROM project_{projectId}_persons
    JOIN project_{projectId}_geocrosswalk ON PRINTF('%07d', project_{projectId}_persons.BG) = project_{projectId}_geocrosswalk.BG'''

    # hhsql = f'''SELECT {selectedVariable},BG FROM project_{projectId}_households'''
    # psql = f'''SELECT {selectedVariable},BG FROM project_{projectId}_persons'''

    if type == "h":
        projects = conn.execute(
            hhsql).fetchall()

        # selectedBGs = [d["BG"] for d in projects if "BG" in d]
        # selectedVarValue = map(lambda d: d[selectedVariable], projects)
        selectedBGs = [d["BG"] for d in projects if "BG" in d]

        print("projects----------", projects[:5], selectedBGs[:5])

        return jsonify(projects)

    elif type == "p":
        projects = conn.execute(
            psql).fetchall()
        # print("projects----------", projects)
        return jsonify(projects)


# @app.route('/project/<projectId>/<selectedVar>/<any(Bgs):selectedBgs>')
# @app.route("/project/<projectId>/<selectedVar>/<any({}):segment>".format(str(selectedBgs)[1:-1]))
@app.route("/project/<projectId>/<selectedVar>/<any('option1', 'option2'):segment>")
def SynPopDataBySelectedBgs(projectId, selectedVar, segment):
    conn = get_db_connection()

    type = selectedVar[0]
    selectedVariable = selectedVar[2:]

    # need to format this remove [] and remove first digits
    shortenbgs = filter(lambda x: x[5:], segment)
    bgs = str(shortenbgs)[1:-1]


# SELECT * FROM project_46_households WHERE BG IN (702001, 702002);

    print("var_Names", selectedVar, type, selectedVariable)

    hhsql = f'''SELECT {selectedVariable} FROM project_{projectId}_households WHERE BG IN ({bgs})'''
    psql = f'''SELECT {selectedVariable} FROM project_{projectId}_persons WHERE BG IN ({bgs})'''
    # hhsql = "SELECT ? FROM project_"+projectId+"_households", (selectedVar,)
    # psql =  "SELECT  ? FROM project_"+projectId+"_persons", (selectedVar,)

    if type == "h":
        projects = conn.execute(
            hhsql).fetchall()
        print("projects----------", projects)
        return jsonify(projects)

    elif type == "p":
        projects = conn.execute(
            psql).fetchall()
        print("projects----------", projects)
        return jsonify(projects)


@app.route('/project/<projectId>/overview')
def countsByProjectId(projectId):
    conn = get_db_connection()

    hhsql = "SELECT COUNT(1) FROM project_"+projectId+"_households"
    psql = "SELECT COUNT(1) FROM project_"+projectId+"_persons"

    households = conn.execute(
        hhsql).fetchall()
    persons = conn.execute(
        psql).fetchall()
    print("total households and persons",
          households[0].values(), persons[0].values())

    return jsonify({"projectID": projectId, "totalHouseholds": households, "totalPersons": persons})
    # return jsonify({ "projectID": projectId,"totalHouseholds": households[0]["COUNT(1)"], "totalPersons": persons[0]["COUNT(1)"]})


@app.route('/project/<projectId>/delete')
def deleteByProjectId(projectId):

    # con = sqlite3.connect("database/activitysimserver.sqlite")
    # cur = con.cursor()
    conn = get_db_connection()
    cur = conn.cursor()
    # delete project from projects and projects_user table
    cur.execute('''DELETE from projects where id = ?''', (projectId,))
    cur.execute(
        '''DELETE from projects_users where project_id = ?''', (projectId,))

   # drop synpop outputs if exist
    dropdatadicsql = f'''drop table if exists project_{projectId}_datadict'''
    dropgeocrosswalksql = f'''drop table if exists project_{projectId}_geocrosswalk'''
    drophhsql = f'''drop table if exists project_{projectId}_households'''
    droppsql = f'''drop table if exists project_{projectId}_persons'''

    cur.execute(dropdatadicsql).fetchall()
    cur.execute(dropgeocrosswalksql).fetchall()
    cur.execute(drophhsql).fetchall()
    cur.execute(droppsql).fetchall()

    conn.commit()

    print("selected project deleted successfully ")

    conn.close()

    return "{\"response\": \"deleted\"}"


@app.route('/pums/psam/<type>/<puma_id>')
def pumaDataById(type, puma_id):
    conn = get_db_connection()
    if type == "h":
        projects = conn.execute(
            'SELECT * FROM psam_h36  WHERE psam_h36.PUMA=?', (puma_id,)).fetchall()
        return jsonify(projects)
    elif type == "p":
        projects = conn.execute(
            'SELECT * FROM psam_p36  WHERE psam_p36.PUMA=?', (puma_id,)).fetchall()
        return jsonify(projects)


@app.route('/project/<projectId>/geometryIds')
def getGeometryIds(projectId):

    conn = get_db_connection()
    cur = conn.cursor()

    # drop synpop outputs if exist
    hhsGeometrySql = f'''select distinct PUMA,BG from project_{projectId}_households'''
    personsGeometrySql = f'''select distinct PUMA, BG from project_{projectId}_persons'''
    crosswalkGeometrySql = f'''select distinct PUMA, BLKGRP from project_{projectId}_geocrosswalk'''

    hhGeometry = cur.execute(hhsGeometrySql).fetchall()
    personsGeometry = cur.execute(personsGeometrySql).fetchall()
    crosswalkGeometry = cur.execute(crosswalkGeometrySql).fetchall()

    # print ( "geometryTable-",hhGeometry )

    return jsonify({"households": hhGeometry, "persons": personsGeometry, "crosswalk": crosswalkGeometry})


@app.route('/project/create', methods=['POST'])
def projectCreate():
    # check user details from db
    print("post success")
    request_data = request.json
    # print(request_data['userId'], request_data)
    # return jsonify(request_data)
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute(
        'INSERT INTO projects values (?, ?, ?, ?)', (None, request_data['project_name'], None, None))

    print("project ID", cursor.lastrowid, " is successfully inserted")

    cursor.execute(
        'INSERT INTO projects_users values (?, ?)', (request_data['userId'], cursor.lastrowid))
    conn.commit()

    return "inserted"


@app.route('/project/create_1', methods=['POST'])
# queue = rq.Queue('popSynth_test3', connection=Redis())
# job = queue.enqueue(projectCreate_1)
def projectCreate_1():
    # check user details from db
    print("post success")
    print(getHash())

    request_data_sql = request.json
    # print("request_data_sql", request_data_sql)
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute(
        'INSERT INTO projects values (?, ?, ?, ?, ?)', (None, request_data_sql['project_name'], None, request_data_sql['status'], None))

    print("project ID", cursor.lastrowid, " is successfully inserted")

    cursor.execute(
        'INSERT INTO projects_users values (?, ?)', (request_data_sql['userId'], cursor.lastrowid))

    conn.commit()

    # if(request.json['project_data']):

    request_data = request.json['project_data']

    # print("crosswork", request_data['crosswork_data'])

    # -- To Do ---
    # 1 - Create a hash code hash()
    # 2 - Create a folder called popsynth_runs/$hash_code

    cwdpath = os.getcwd()
    print("The current working directory is %s" % cwdpath)

    # parent_dir = cwdpath
    # directory = "/popsynth_runs/" + getHash()
    # path = os.path.join(parent_dir,"/popsynth_runs/", directory)

    # parent_dir = "/home/jin/AVAIL/code/avl-modeler2/server/popsynth_runs/"
    parent_dir = os.getcwd() + '/popsynth_runs/'
    # directory =  getHash()
    directory = str(cursor.lastrowid)
    path = os.path.join(parent_dir, directory)

    # popsynth.deleteFolder(path)

    # 3 - in the new folder create config, data and output folders
    subfolder_names = ["configs", "data", "output"]
    # subfolder_names = ["configs", "data"]

    file_names = ["controls", "geo_cross_walk",  "control_totals_tract",
                  "control_totals_bg", "seed_households", "seed_persons", "settings"]

    # file_names = ["controls","geo_cross_walk",  "control_totals_tract", "control_totals_bg","seed_households", "seed_persons"]

    print('keys', request_data.keys())

    # splite json input data to single json file

    config_data = request_data['control_config']
    crosswork_data = request_data['crosswork_data']

    control_data = request_data['control_data']
    control_tracts = control_data['control_tracts']
    control_bgs = control_data['control_bgs']

    seed_data = request_data['seed_data']
    seed_household = seed_data['household']
    seed_person = seed_data['person']

    settings = request_data['setting']

    if not os.path.exists(path):
        #  os.mkdir(os.path.join(path, 'output'))

        for i, subfolder_name in enumerate(subfolder_names):

            # make folders
            os.makedirs(os.path.join(path, subfolder_name))

    # 4 - write output files from json data to the matching folder

            # config folder
            if i == 0:

                complete_path = os.path.join(
                    path, subfolder_name, file_names[0]+".csv")

                data_file = open(complete_path, "w")

                # create the csv writer object
                csv_writer = csv.writer(data_file)

                # Counter variable used for writing
                # headers to the CSV file
                count = 0

                for data in config_data:
                    if count == 0:

                        # Writing headers of CSV file
                        header = data.keys()
                        csv_writer.writerow(header)
                        count += 1

                    # Writing data of CSV file
                    csv_writer.writerow(data.values())

                data_file.close()

            # do yaml here

                complete_path_yaml = os.path.join(
                    path, subfolder_name, file_names[6]+".yaml")
                with open(complete_path_yaml, 'w') as file:
                    yaml.dump(settings, file)
                    print('settings.yaml', yaml.dump(settings))

            # data folder
            elif i == 1:

                path1 = os.path.join(path, subfolder_name,
                                     file_names[1]+".csv")
                path2 = os.path.join(path, subfolder_name,
                                     file_names[2]+".csv")
                path3 = os.path.join(path, subfolder_name,
                                     file_names[3]+".csv")
                path4 = os.path.join(path, subfolder_name,
                                     file_names[4]+".csv")
                path5 = os.path.join(path, subfolder_name,
                                     file_names[5]+".csv")

                complete_path = [path1, path2, path3, path4, path5]
                data_data = [crosswork_data, control_tracts,
                             control_bgs, seed_household, seed_person]

                for i, pathi in enumerate(complete_path):

                    data_file = open(pathi, "w")

                    # create the csv writer object
                    csv_writer = csv.writer(data_file)

                    # Counter variable used for writing
                    # headers to the CSV file
                    count = 0

                    for data in data_data[i]:
                        if count == 0:

                            # Writing headers of CSV file
                            header = data.keys()
                            csv_writer.writerow(header)
                            count += 1

                        # Writing data of CSV file
                        csv_writer.writerow(data.values())

                        # os.path.join( "/home/jin/AVAIL/code/avl-modeler2/server/popsynth_runs/configs", data_file)

                    data_file.close()


#   create a task for RQ

    # before we queue our task
    # insert new record into sqlite projects table with project name
    # this should be able to return a id number from the insert
    # use the project ID instead of the hash to create the new folder
    # add new column status to project table, set status to processing

    # return to the client the object #{id: $project_id, project_name: '', status: 'processing' }

    queue = rq.Queue('rq_popsynth', connection=Redis())

    parser = argparse.ArgumentParser()
    add_run_args(parser)
    args = parser.parse_args(
        ['--working_dir', os.getcwd() + '/popsynth_runs/' + directory])
    # print('testing args', args)
    # job = queue.enqueue(run(args))
    #
    project_id = directory
    project_name = request_data_sql['project_name']
    status = request_data_sql['status']

    job = queue.enqueue('tasks.popsynth.run_popsynth', args, project_id)

    return "{\"reponse\": \"success\"}"
    # return "{\"id\": project_id, \"project_name\": project_name, \"status\": status}"
