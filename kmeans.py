import pandas as pd
import numpy as np
import kmeansmodule as km


def create_vector_dataframe(file1, file2):

    db1 = pd.read_csv(file1, header=None)
    db1_number_of_data_points = len(db1.columns) - 1
    cols = ['index']
    cols.extend([str(i) for i in range(db1_number_of_data_points)])
    db1.columns = cols
    db1['index'] = db1['index'].astype(int)
    db1 = db1.set_index('index')
    #print(db1.head())

    db2 = pd.read_csv(file2, header=None)
    db2_number_of_data_points = len(db2.columns) - 1
    cols = ['index']
    cols.extend([str(i) for i in range(db1_number_of_data_points,
                db1_number_of_data_points + db2_number_of_data_points)])
    db2.columns = cols
    db2['index'] = db2['index'].astype(int)
    db2 = db2.set_index('index')
    #print(db2.head())
    db = pd.merge(db1, db2, left_index=True, right_index=True)
    db = db.sort_index()
    return db


def calc_dist(vector1_index, vector2_index, vector_dataframe):  # complete
    temp = vector_dataframe.loc[[vector1_index, vector2_index]] # dataframe with 2 vectors
    return np.linalg.norm(temp.iloc[1] - temp.iloc[0])

# centroid list must not be empty


def compute_dx(vector_index, vector_dataframe, centroid_index_list):
    min_centroid_index = None
    min_centroid_dist = None
    for centroid_index in centroid_index_list:
        temp = calc_dist(vector_index, centroid_index, vector_dataframe)
        if (min_centroid_index is None or min_centroid_dist > temp):
            min_centroid_dist = temp
            min_centroid_index = centroid_index
    return min_centroid_dist


def compute_dx_tuple(vector_dataframe, centroid_index_list):
    #print([vector_index for vector_index in db.index.values])
    return [compute_dx(vector_index, vector_dataframe, centroid_index_list) for vector_index in db.index.values]


def compute_weights(vector_dataframe, centroid_index_list):
    dx_tuple = compute_dx_tuple(vector_dataframe, centroid_index_list)
    #print(dx_tuple)
    return dx_tuple/sum(dx_tuple)

def compute_centroids(vector_dataframe, k):
    centroid_index_list = []
    for _ in range(k):
        if len(centroid_index_list) == 0:
            temp = np.random.choice(db.index.values)
        else:
            weight_list = compute_weights(vector_dataframe, centroid_index_list)
            #print(weight_list)
            #print(sum(weight_list))
            temp = np.random.choice(db.index.values, p = weight_list)
        centroid_index_list.append(temp)
    return centroid_index_list

# add error incase k is bigger than n?

def create_coor_array(vector_dataframe):
    coor_arr = []
    for i in db.index.values:
        coor_arr.extend(list(vector_dataframe.iloc[i]))
    return coor_arr

if __name__ == "__main__":
    np.random.seed(0)
    iter = 300
    eps = 0.01
    k = 3
    file1 = "tests\input_1_db_1.txt"
    file2 = "tests\input_1_db_2.txt"
    db = create_vector_dataframe(file1, file2)
    temp_coor_arr = create_coor_array(db)
    temp = compute_centroids(db, k)
    #calc_dist(0,1,db)
    vec_num = len(db.index.values) # rows
    vec_size = len(db.columns) # col
    results = km.fit(iter, eps, k, vec_num, vec_size, temp_coor_arr, temp)
    print(temp)
    print(results)
    #print(db.loc[temp])
    #print(db.loc[temp].to_numpy())
