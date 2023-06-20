from sklearn.datasets import load_iris
from sklearn.cluster import KMeans
import numpy as np
import matplotlib.pyplot as plt

def calc(db, k):
    kmeans = KMeans(init = 'k-means++', n_clusters=k, n_init = 'auto', random_state=0)
    kmeans.fit_predict(db)
    return (kmeans.inertia_)
        
if __name__ == "__main__":
    iris_dataset = load_iris()
    db = iris_dataset.data
    y = [calc(db, k+1) for k in range(10)]
        
    x = [k+1 for k in range(10)]
    fig, ax = plt.subplots()
    ax.plot(x, y, linewidth=2.0)
    ax.set(xticks=np.arange(1, 11))
    plt.ylabel("Average Dispersion")
    plt.xlabel("K")
    plt.title('Elbow Method for selection of optimal "K" clusters')
    plt.plot(3, y[2], marker = 'o', ms = 20, mfc = 'none', mec='black', mew=2)
    plt.annotate("Elbow Point", (3 + 0.3, y[2] + 50))
    plt.savefig("elbow.png")      