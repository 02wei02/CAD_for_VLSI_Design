#!/usr/bin/env python
# coding: utf-8
# Lab for IR drop prediction using XGBoost
# You don't have to modify this file

# Import packages
import xgboost as xgb
import pandas as pd
import numpy as np
import pickle
import time
import subprocess

# For quality check
HIDDEN = False

# Circuit name
DESIGN="MEMC"

# Function to predict result
def predict_result(raw_data, pattern_num, model):
    
    # Input feature, this should be same as training feature
    feature_name = np.load("./feature_name.npy",allow_pickle=True)
    X = raw_data.loc[:, raw_data.columns.isin(feature_name)]

    # Golden IR-drop
    Y = raw_data["IR-drop"]
    dpredict = xgb.DMatrix(X,feature_names=feature_name)

    # Output IR-drop of every cell
    predicted_data =  pd.DataFrame()
    predicted_data['ptn'+str(pattern_num+1)+'_golden'] = raw_data['IR-drop'].values
    predicted_data['ptn'+str(pattern_num+1)+'_predict'] = model.predict(dpredict)
    predicted_data.to_csv("./data/predicted_result_"+str(pattern_num+1)+".csv")

# Function to load prediction result and golden data to dataframe
def load_all_data(test_set) :
    all_cells_golden = pd.DataFrame()
    all_cells_predict = pd.DataFrame()

    for pattern in test_set:
        temp = pd.read_csv("./data/predicted_result_"+str(pattern+1)+".csv")
        all_cells_golden['ptn'+str(pattern+1)] = temp['ptn'+str(pattern+1)+'_golden']
        all_cells_predict['ptn'+str(pattern+1)] = temp['ptn'+str(pattern+1)+'_predict']
    
    return 1000*(0.95-all_cells_golden), 1000*(0.95-all_cells_predict)


if __name__ == '__main__':

    # Require trained model to use below options
    TRAINED_MODEL = "./model/PA4_Model.dat"
    
    # Set testing set
    if HIDDEN:
        TEST_SET = np.arange(90, 100)
    else:
        TEST_SET = np.arange(80, 90)
    print("Test set is: ",TEST_SET)
    
    # Start to predict
    start = time.time()
    
    # Create file to store prediction 
    p1 = subprocess.Popen(["mkdir -p ./data/predicted_result"], stdout=subprocess.PIPE, shell=True)
    p1.wait()
    
    # Increase Pandas display number
    pd.set_option('display.max_rows', 300)

    # Load the model
    model = pickle.load(open(TRAINED_MODEL, "rb"))
    model.set_param({"predictor": "cpu_predictor"})

    start = time.time()
    for pattern_num in TEST_SET:
        FILE_NAME = DESIGN+'_'+str(pattern_num+1)
        DIR = "Hidden" if HIDDEN else "Public"
        raw_data = pd.read_csv("/home/CAD112/PA4/Testing."+DIR+'/'+FILE_NAME+'.csv')
        print("Testing", FILE_NAME+'.csv')
        predict_result(raw_data, pattern_num, model)
    end = time.time()
    # Output total time
    print("total time: ", end-start)
    print("prediction done")

    # Read prediction and golden IR drop
    TEST_all_cells_golden, TEST_all_cells_predict = load_all_data(TEST_SET)
    print("all_cells_golden shape:", TEST_all_cells_golden.shape)

    # TEST_all_cells_golden = all_cells_golden
    # TEST_all_cells_predict = all_cells_predict
    #  Calculate MAE, MaxE, CC, NRMSE
    print("MAE(mV): ",format(np.mean(np.abs(TEST_all_cells_predict.values-TEST_all_cells_golden.values)),".2f"),"mV")
    print("MaxE(mV): ",format(np.max(TEST_all_cells_predict.values-TEST_all_cells_golden.values),".2f"),'mV')
    print("CC: ", format(np.corrcoef(TEST_all_cells_golden.values.flatten(),TEST_all_cells_predict.values.flatten())[0,1],".2f"))
    print("NRMSE(%): ",format(np.sqrt(np.mean((TEST_all_cells_predict.values-TEST_all_cells_golden.values)**2))/np.mean(TEST_all_cells_predict.values)*100,".2f"),'%')
