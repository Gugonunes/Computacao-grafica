
# Python code for 1-D random walk.
import random
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
 
#criando a matriz do mapa
matriz = np.zeros((100,100))
#incluir pontos iniciais aleatorios com valores aleatorios entre 0 e 5
for i in range(100):
    for j in range(100):
        if random.randint(0, 100) == 1:
            matriz[i][j] = random.randint(0, 10)
            #atribua esse valor aos vizinhos também
            # if i > 0 and i < 99 and j > 0 and j < 99:
            #     matriz[i-1][j] = 0.5
            #     matriz[i+1][j] = 0.5
            #     matriz[i][j-1] = 0.5
            #     matriz[i][j+1] = 0.5

def chooseValue(matriz, i, j):
    #verificar se os vizinhos estão dentro da matriz
    if i > 0 and i < 99 and j > 0 and j < 99:
        #identificar os vizinhos
        vizinhoCima = matriz[i-1][j]
        vizinhoBaixo = matriz[i+1][j]
        vizinhoEsquerda = matriz[i][j-1]
        vizinhoDireita = matriz[i][j+1]
        
        #comparar todos os vizinhos e encontrar o maior/menor valor
        valorMaior = max(vizinhoCima, vizinhoBaixo, vizinhoEsquerda, vizinhoDireita)
        valorMenor = min(vizinhoCima, vizinhoBaixo, vizinhoEsquerda, vizinhoDireita)

        return (valorMenor+valorMaior)/2
    else:
        return 0

def randomWalk():
    for i in range(100):
        for j in range(100):
            matriz[i][j] = chooseValue(matriz, i, j)

for i in range(75):
    randomWalk()

# Cria um grid de coordenadas para a superfície
x = np.arange(matriz.shape[1])
y = np.arange(matriz.shape[0])
x, y = np.meshgrid(x, y)

# Cria uma figura com duas sub-figuras
fig, (ax1, ax2) = plt.subplots(1, 2)

# Plota a imagem em uma sub-figura
ax1.imshow(matriz, cmap='viridis')
ax1.set_title('Imagem')

# Plota a superfície em outra sub-figura
ax2 = fig.add_subplot(122, projection='3d')
ax2.plot_surface(x, y, matriz, cmap='viridis', edgecolor='black')
ax2.set_title('Superfície')

# Define os limites do eixo z
# ax2.set_zlim(-6, 4)

# Exibe a figura com as duas sub-figuras em janelas diferentes, mas ao mesmo tempo
plt.show()

