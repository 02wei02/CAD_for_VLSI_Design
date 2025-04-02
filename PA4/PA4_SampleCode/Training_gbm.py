#!/usr/bin/env python
# coding: utf-8
# Lab for IR drop prediction using LightGBM

# Import packages
from sklearn.datasets import dump_svmlight_file
import lightgbm as lgb
import pandas as pd
import numpy as np
import os, shutil
import subprocess
import pickle
import time

# Circuit name
DESIGN = "MEMC"

# Debug setting
DEBUG = True

# Create directory to store model and data
p1 = subprocess.Popen(["mkdir -p ./model"], stdout=subprocess.PIPE, shell=True)
p1 = subprocess.Popen(["mkdir -p ./data/train/"], stdout=subprocess.PIPE, shell=True)
p1 = subprocess.Popen(["mkdir -p ./data/val"], stdout=subprocess.PIPE, shell=True)

# Wait until directory is created
p1.wait()

# ******************************************************************
# TODO 1: Set training and validation dataset
TRAINING_SET = np.random.choice(78, 20, replace=False)
# np.arange(10)
VALIDATION_SET = np.array([80])
print("Training set is: ", TRAINING_SET)
print("Validation set is: ", VALIDATION_SET)
# END of TODO 1
# ******************************************************************

# Get all input features
def get_feature():
    # ******************************************************************
    # TODO 2: Select the features for training
    feature_name = ['y', 'SPR', 'Reff', 'x', 'w', 'Pleak', 'h', 'Ipeak']
    print(feature_name)
    np.save("./feature_name.npy", np.array(feature_name))
    # END of TODO 2
    # ******************************************************************

# Save training data to temp files
def dump_file(raw_data, pattern_num, dir_name):
    feature_name = np.load("./feature_name.npy",allow_pickle=True)
    X = raw_data.loc[:, raw_data.columns.isin(feature_name)]

    # Golden IR-drop
    Y = raw_data["IR-drop"]
    dump_svmlight_file(X,Y,"./data/"+dir_name+"/"+DESIGN+"_"+str(pattern_num)+".dat")

# Split training & validation set
def load_data(training_set, validation_set):
    DataSet_Path = "/home/CAD112/PA4/Training/"
    print('Loading dataset...')
    train_name_dict = [str(i+1) for i in training_set]
    val_name_dict = [str(i) for i in validation_set]
    feature_name = np.load("./feature_name.npy", allow_pickle=True)

    train_X = []
    train_y = []
    val_X = []
    val_y = []

    for pattern_num in train_name_dict:
        FILE_STR = DESIGN + '_' + str(pattern_num)
        all_data = pd.read_csv(DataSet_Path + FILE_STR + '.csv')
        X_train = all_data[feature_name]
        y_train = all_data["IR-drop"]
        train_X.append(X_train)
        train_y.append(y_train)
        print(FILE_STR + '.csv loaded for training...')

    for pattern_num in val_name_dict:
        FILE_STR = DESIGN + '_' + str(pattern_num)
        all_data = pd.read_csv(DataSet_Path + FILE_STR + '.csv')
        X_val = all_data[feature_name]
        y_val = all_data["IR-drop"]
        val_X.append(X_val)
        val_y.append(y_val)
        print(FILE_STR + '.csv loaded for validation...')
    
    # Combine all training and validation data
    X_train = pd.concat(train_X)
    y_train = pd.concat(train_y)
    X_val = pd.concat(val_X)
    y_val = pd.concat(val_y)

    dtrain = lgb.Dataset(X_train, label=y_train)
    dval = lgb.Dataset(X_val, label=y_val)

    return dtrain, dval


def training(dtrain, dval):
    # Define model parameter
    param = {
        'max_depth': 8,
        'learning_rate': 0.1,  # 通常學習率要較低
        'metric': "rmse",
        'objective': "regression",
        'lambda_l1': 1,
        'lambda_l2': 1,
        'tree_learner': 'serial',
        'num_leaves': 2**8,  # 這裡設置為 2^max_depth
        'min_data_in_leaf': 10,
        'feature_fraction': 0.8,
        'bagging_fraction': 0.8,
        'bagging_freq': 5,
        'verbose': 0,
        'min_gain_to_split': 0.01,  # 設置一個較小的 min_gain_to_split
        'early_stopping_round': 10,
    }

    # Start training model
    model = lgb.train(params=param,
                      train_set=dtrain,
                      valid_sets=dval)

    # Save the model
    model.save_model("./model/PA4_Model.cbm")

    return model


if __name__ == '__main__':
    # Get input feature
    get_feature()
    
    # Data preprocessing
    dtrain, dval = load_data(TRAINING_SET, VALIDATION_SET)

    # Train the model
    print("start training")
    start = time.time()
    model = training(dtrain, dval)
    end = time.time()

    print("total time: ", end - start)
    print("model produced successfully")

    # Clear all temp files
    shutil.rmtree("./data/train/")
    shutil.rmtree("./data/val/")
