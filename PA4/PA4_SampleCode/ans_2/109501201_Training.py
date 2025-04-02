#!/usr/bin/env python
# coding: utf-8
# Lab for IR drop prediction using XGBoost

# Import packages
from sklearn.datasets import dump_svmlight_file
import xgboost as xgb
import pandas as pd
import numpy as np
import os, shutil
import subprocess
import pickle
import time

# Curcuit name
DESIGN="MEMC"

# Debug setting
DEBUG = True

# Create directory to store model and data
p1 = subprocess.Popen(["mkdir -p ./model"], stdout=subprocess.PIPE, shell=True)
p1 = subprocess.Popen(["mkdir -p ./data/train/"], stdout=subprocess.PIPE, shell=True)
p1 = subprocess.Popen(["mkdir -p ./data/val"], stdout=subprocess.PIPE, shell=True)

# Wait until dierctory is created
p1.wait()


# ******************************************************************
# TODO 1: Set training and validation dataset
total_numbers = np.arange(80)
TRAINING_SET = np.random.choice(total_numbers, 70, replace=False)
# np.arange(10)
 # *** your code here ***
VALIDATION_SET = np.setdiff1d(total_numbers, TRAINING_SET) # *** your code here ***
print("Training set is: ",TRAINING_SET)
print("Validation set is: ", VALIDATION_SET)

# END of TODO 1
# ******************************************************************


# Get all input feature
def get_feature():

    # ******************************************************************
    # TODO 2: Select the features for training
    # Sample: feature_name = ["x", "y", "w", "h"]
    # Note that you should not put "IR-drop" as your training features
    
    feature_name = ['y','SPR','Reff','x','Tarrival','TCinternal','w','Cell type','Pinternal','Pleak','Ipeak','Ttransition','Cload'] # *** your code here ***
    print(feature_name)
    np.save("./feature_name.npy",np.array(feature_name))

    # END of TODO 2
    # ******************************************************************


# Save training data to temp svm temp files
def dump_file(raw_data, pattern_num, dir_name):
    
    feature_name = np.load("./feature_name.npy",allow_pickle=True)
    X = raw_data.loc[:, raw_data.columns.isin(feature_name)]

    # Golden IR-drop
    Y = raw_data["IR-drop"]
    dump_svmlight_file(X,Y,"./data/"+dir_name+"/"+DESIGN+"_"+str(pattern_num)+".dat")


# split training & validation set 
def load_data(training_set, validation_set):
    DataSet_Path = "/home/CAD112/PA4/Training/"
    print('Loading dataset...')
    train_name_dict = [str(i+1) for i in training_set]
    val_name_dict = [str(i+1) for i in validation_set]
    feature_name = np.load("./feature_name.npy",allow_pickle=True)
    
    for pattern_num in train_name_dict:
        DIR = 'train'
        FILE_STR = DESIGN+'_'+str(pattern_num)
        all_data = pd.read_csv(DataSet_Path+FILE_STR+'.csv')
        dump_file(all_data, pattern_num, DIR)
        print(FILE_STR+'.csv loaded...')


    for pattern_num in validation_set:
        DIR = 'val'
        FILE_STR = DESIGN+'_'+str(pattern_num)
        all_data = pd.read_csv(DataSet_Path+FILE_STR+'.csv')
        dump_file(all_data, pattern_num, DIR)
        print(FILE_STR+'.csv loaded...')
    
    # All files under train folder are training data
    dtrain = xgb.DMatrix('./data/train/',feature_names=feature_name)

    # All files under val folder are validation data
    dval = xgb.DMatrix('./data/val',feature_names=feature_name)

    return dtrain,dval


def training(dtrain, dval):
    #    Different objective to use
    #      reg:squarederror: regression with squared loss
    #      reg:squaredlogerror: regression with squared log loss
    #      reg:logistic: logistic regression
    #      reg:pseudohubererror: regression with Pseudo Huber loss, a twice differentiable alternative to absolute loss.
    # ******************************************************************
    # TODO 3: Define model parameter
    # You can add more parameter settings if needed

    param = {
        'max_depth': 8,  # 樹的最大深度
        'eta': 0.4,  # 步長
        'eval_metric': "rmse",  # 評價指標
        'objective': "reg:squarederror",  # 目標函數
        'lambda': 1,  # L2 正則化
        'n_estimators': 500,  # 樹的數量
        # 'subsample': 0.8,  # 子樣本比例
        # 'colsample_bytree': 0.8,  # 每棵樹的特徵比例
        # 'colsample_bylevel': 0.8,  # 每層的特徵比例
        # 'colsample_bynode': 0.8,  # 每個節點的特徵比例
        # 'gamma': 0.1,  # 最小損失減少
        # 'min_child_weight': 1,  # 最小樣本權重和
        # 'alpha': 0.1  # L1 正則化
    }
    # END of TODO 3
    # ******************************************************************


    # The first value in watchlist is for early stopping
    watchlist = [(dtrain, 'train'),(dval, 'eval')]


    # Number of training iteration, i.e. maximum tree number in random forest
    # ******************************************************************
    # TODO 4: Determine training itreation

    num_round = 50 # *** your number here *** 

    # END of TODO 4
    # ******************************************************************


    # Start training model
    #  params: booster parameter
    #  dtrain: training data
    #  evals:  validation data
    #  early_stopping_rounds: stop if the result is not better for # iterations
    #  you can adjust this part if you want
    model = xgb.train(params = param, 
                     dtrain = dtrain, 
                     num_boost_round = num_round, 
                     evals = watchlist, 
                     early_stopping_rounds = 5
                     )
    
    
    # Save the model
    pickle.dump(model, open("./model/PA4_Model.dat", "wb"))
    return model


if __name__ == '__main__' :
    # Get input feature
    get_feature()
    
    # Data preprocessing
    dtrain,dval = load_data(TRAINING_SET,VALIDATION_SET)

    # Train the model
    print("start tranining")
    start = time.time()
    model = training(dtrain,dval)
    end = time.time()

    print("total time: ", end-start)
    print("model produced sucessfully")

    # Clear all temp files
    shutil.rmtree("./data/train/")
    shutil.rmtree("./data/val/")
