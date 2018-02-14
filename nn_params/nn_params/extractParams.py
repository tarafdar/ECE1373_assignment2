#/usr/bin/env python

import caffe
import string
import os
import numpy as np
from scipy.misc import imread
from PIL import Image

net = caffe.Net('/opt/caffe/models/bvlc_googlenet/deploy.prototxt' , '/opt/caffe/models/bvlc_googlenet/bvlc_googlenet.caffemodel', caffe.TEST)

layers = ['conv1/7x7_s2', 'conv2/3x3_reduce']
pad = [3,0]
batches = [1,1]
input_dim = [3,64]
input_x = [224,112]
input_y = [224,112]

input_batch_size = 1
image = []

for i in range(1, input_batch_size+1):
    image_name = 'cars_test/' + str(i).zfill(5) + '.jpg' 
    img = Image.open(image_name)
    img =  img.resize((224,224), Image.BILINEAR)
    img.save(image_name)
    image.append(imread(image_name))

image = np.asarray([image])
image = image.reshape(input_batch_size,3,224,224)
array_image = np.asarray([image])
net.blobs['data'].data[...] = array_image


num_layer = 0
for layer in layers:

    dirname = string.replace(layer,'/','_')
    if not os.path.exists(dirname):
        os.makedirs(dirname)

    weight_name = dirname  + '/weights'
    weight_file = open(weight_name, 'wb')
    weight_file.close()
    weight_file = open(weight_name, 'a+')

    W =  net.params[layer][0].data[...]
    weight_file.write(bytes(len(W.shape))) 
    weight_file.write(bytes(W.shape))
    weight_file.write(np.asarray(W.tobytes()))
    weight_file.close()
    print "weight_blob shape " + str(W.shape) 
    weight_file.close()
    
    bias_name = dirname  + '/biases'
    bias_file = open(bias_name, 'wb')
    bias_file.close()
    bias_file = open(bias_name, 'a+')
    b =  net.params[layer][1].data[...]
    bias_file.write(bytes(len(b.shape))) 
    bias_file.write(bytes(b.shape))
    bias_file.write(np.asarray(b.tobytes()))
    bias_file.close()
    print "bias_blob shape " + str(b.shape) 
    bias_file.close()

    input_name = dirname + '/input'
    input_file = open(input_name, 'wb')
    input_file.close()
    input_file = open(input_name, 'a+')

    	
    bottom_name = net.bottom_names[layer][0]

    input_blob = net.blobs[bottom_name].data[...]
    pad_arr = np.zeros((batches[num_layer], input_dim[num_layer], input_x[num_layer]+2*pad[num_layer], input_y[num_layer]+2*pad[num_layer])).astype(np.float32)

    print input_blob.shape
    for i in range(0, batches[num_layer]):
    	if (pad[num_layer]>0):
        	pad_arr[i:,:,pad[num_layer]:-pad[num_layer],pad[num_layer]:-pad[num_layer]] = input_blob[i] 


    input_file.write(bytes(len(pad_arr.shape))) 
    input_file.write(bytes(pad_arr.shape))
    input_file.write(pad_arr.tobytes())
    input_file.close()
    
    net.forward()
    
    

    print "input_blob shape " + str(array_image.shape) 
    print "padded input_blob shape " + str(pad_arr.shape) 
    
    output_name = dirname + '/output'
    output_file = open(output_name, 'wb')
    output_file.close()
    
    output_file = open(output_name, 'a+')
    output_blob = net.blobs[layer].data[...]
   
    output_file.write(bytes(len(output_blob.shape))) 
    output_file.write(bytes(pad_arr.shape))
    output_file.write(np.asarray(pad_arr.tobytes()))
    output_file.close()
    print "output_blob shape " + str(output_blob.shape)
    output_file.close()
    num_layer = num_layer+1
