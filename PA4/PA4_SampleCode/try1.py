import xgboost as xgb
from sklearn.model_selection import train_test_split
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 讀取數據
DataSet_Path = "/home/CAD112/PA4/Training/"
FILE_STR = "MEMC_1"
data = pd.read_csv(DataSet_Path + FILE_STR + '.csv')

# 提取特徵和標籤
feature_name = ["x", "y", "w", "h", "Reff", "SPR", "Pleak", "Cload", "Pir", "Ipeak", "Pswitch", "Pinternal", "Ttransition","Cell type","Pic1", "Pic2", "TCinput","TCoutput","TCinternal", "Tarrival"]
X = data[feature_name]
y = data["IR-drop"]

# 拆分訓練集和測試集
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 訓練模型
model = xgb.XGBRegressor(objective='reg:squarederror')
model.fit(X_train, y_train)

# 獲取特徵重要性
importance = model.feature_importances_
importance_df = pd.DataFrame({
    'Feature': feature_name,
    'Importance': importance
})

# 按重要性排序
importance_df = importance_df.sort_values(by='Importance', ascending=False)
print(importance_df)

# 可視化特徵重要性
plt.figure(figsize=(10, 6))
plt.barh(importance_df['Feature'], importance_df['Importance'])
plt.xlabel('Importance')
plt.ylabel('Feature')
plt.title('Feature Importance')
plt.gca().invert_yaxis()
plt.show()