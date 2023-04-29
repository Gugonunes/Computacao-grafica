import numpy as np
from PIL import Image

GREYSCALE = False
img_name = '1665_girl_with_a_pearl_earring_sm.jpg'

# Read in the image, convert to greyscale.
#img = Image.open('Dither/rimg-2.jpg')
img = Image.open('Dither/1665_girl_with_a_pearl_earring_sm.jpg')
if GREYSCALE:
    img = img.convert('L')

width, height = img.size
new_width = 400
new_height = int(height * new_width / width)
img = img.resize((new_width, new_height), Image.ANTIALIAS)

def get_new_val(old_val, nc):
    return np.round(old_val * (nc + 1)) / (nc + 1)

def palette_reduce(img, nc):
    """Simple palette reduction without dithering."""
    arr = np.array(img, dtype=float) / 255
    arr = get_new_val(arr, nc)

    carr = np.array(arr/np.max(arr) * 255, dtype=np.uint8)
    return Image.fromarray(carr)

nc = 2
# print('nc =', nc)
rim = palette_reduce(img, nc)
rim.save('rimg-{}.jpg'.format(nc))

#dither reverso:

img2 = Image.open('rimg-2.jpg')
def dither_reverse(img, img2, nc):
    arr1 = np.array(img, dtype=float) / 255
    print(arr1[0][0])
    arr2 = np.array(img2, dtype=float) / 255
    
    for i in range(len(arr1)):
        for j in range(len(arr1[i])):
            for k in range(len(arr1[i][j])):
                if arr1[i][j][k] > arr2[i][j][k]:
                    arr1[i][j][k] = arr2[i][j][k] + 1/(nc + 4)
                else:
                    arr1[i][j][k] = np.average(arr2[i][j][k])
    
    carr = np.array(arr1/np.max(arr1) * 255, dtype=np.uint8)
    return Image.fromarray(carr)

# print('nc =', nc)
rim = dither_reverse(img, img2, nc)
rim.save('reverse-rimg-{}.jpg'.format(nc))