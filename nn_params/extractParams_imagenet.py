__author__ = 'pittnuts'
'''
Main script to run classification/test/prediction/evaluation
'''
import os
import string
import numpy as np
import struct
import matplotlib.pyplot as plt
from scipy.io import *
from PIL import Image
import caffe
import sys
import lmdb
from caffe.proto import caffe_pb2
from os import system
import time
import argparse
from pprint import pprint
from google.protobuf import text_format






total_batches=500

#model  = '/opt/caffe/models/bvlc_alexnet/deploy.prototxt'
#weights  = '/opt/caffe/models/bvlc_alexnet/bvlc_alexnet.caffemodel'
#mean_file = '/opt/caffe/data/ilsvrc12/imagenet_mean_227.binaryproto'
#image_root = 'alexnet_batches/'
#imagenet_val_path = '/opt/caffe/examples/imagenet/ilsvrc12_train_227_lmdb'
model  = '/opt/caffe/models/vgg16/VGG_ILSVRC_16_layers_deploy_update.prototxt'
weights  = '/opt/caffe/models/vgg16/VGG_ILSVRC_16_layers_update.caffemodel'
mean_file = '/opt/caffe/data/ilsvrc12/imagenet_mean_224.binaryproto'
image_root = '../data/vgg_batches/'
imagenet_val_path = '/opt/caffe/examples/imagenet/ilsvrc12_train_224_lmdb'
caffe.set_mode_cpu()
parse_net = caffe_pb2.NetParameter()
with open(model, 'r') as f:
  text_format.Merge(f.read(), parse_net)
idx_map = {str(x.name):i for i,x in enumerate(parse_net.layer) if x.type in {'Convolution', 'Pooling', 'InnerProduct'}}


net = caffe.Net(model, weights, caffe.TEST)

batch_size = net.blobs['data'].data[...].shape[0]
layers = idx_map.keys()
#layers = ['fc8']
labels_set = set()
lmdb_env = lmdb.open(imagenet_val_path)
lmdb_txn = lmdb_env.begin()
lmdb_cursor = lmdb_txn.cursor()
mean_blob = caffe.proto.caffe_pb2.BlobProto()
mean_data = open( mean_file , 'rb' ).read()
mean_blob.ParseFromString(mean_data)
pixel_mean = np.array( caffe.io.blobproto_to_array(mean_blob) )


avg_time = 0
label = np.zeros((batch_size,1))
image_count = 0



num_layer = 0
num_batch = 0
for key, value in lmdb_cursor:
    datum = caffe.proto.caffe_pb2.Datum()
    datum.ParseFromString(value)
    label[image_count%batch_size,0] = int(datum.label)
    image = caffe.io.datum_to_array(datum)
    image = image.astype(np.uint8)
    image = image-pixel_mean.mean(0)
    net.blobs['data'].data[image_count%batch_size] = image#image-pixel_mean
    print "Batch Size " + str(batch_size)
    if num_batch >= total_batches:
	break
    if image_count % batch_size == (batch_size-1):
    	print "Processing Batch " + str(num_batch)
	
        out = net.forward()
	#get labels of prediction
    	plabel = out['prob'][:].argmax(axis=1)

	#Create directory of all batches
	dirnameBatch = image_root + "batch_" + str(num_batch) + "/" 
	if not os.path.exists(dirnameBatch):
	    os.makedirs(dirnameBatch)
	label_name = dirnameBatch + "labels"
	label_file = open(label_name, 'wb')
        label.tofile(label_file)
	for layer in layers:
    	    print "Extracting Layer: ", layer
            # Layer Parameters
    	    parse_layer = parse_net.layer[idx_map[layer]]
    	    layer_params = {'name':layer,
		    'type':str(parse_layer.type)}
	   
    	    # Create output directory
	    dirname = dirnameBatch + string.replace(layer,'/','_')
	    if not os.path.exists(dirname):
	        os.makedirs(dirname)

    	    # Extract parameters for Convolution and InnerProduct layers
            if parse_layer.type != 'Pooling':
            	W = net.params[layer][0].data[...]
            	b = net.params[layer][1].data[...]
            	with open(dirname + '/weights', 'wb') as f:
                    #f.write(struct.pack('i' * (1+len(W.shape)), len(W.shape), *W.shape))
                    W.tofile(f)
                with open(dirname + '/biases', 'wb') as f:
                    b.tofile(f)
            else:
              W = None
              b = None

    	    pad = 0
    	    if parse_layer.type == 'Convolution':
      	        pad = parse_layer.convolution_param.pad
      	        stride = parse_layer.convolution_param.stride
      	        kern_size = parse_layer.convolution_param.kernel_size
      	        pad = 1 if not pad else int(pad[0])
      	        stride = 1 if not stride else int(stride[0])
      	        kern_size = 1 if not kern_size else int(kern_size[0])
      	        layer_params['pad'] = pad 
      	        layer_params['stride'] = stride
      	        layer_params['kernel_size'] = kern_size
    	    # Extract Pooling Params
    	    elif parse_layer.type == 'Pooling':
      	        pad = parse_layer.pooling_param.pad
      	        stride = parse_layer.pooling_param.stride
      	        kern_size = parse_layer.pooling_param.kernel_size
      	        layer_params['pad'] = pad 
      	        layer_params['stride'] = stride
      	        layer_params['kernel_size'] = kern_size


    	    # Get Input/Output blobs
    	    bottom_name = net.bottom_names[layer][0]
    	    input_blob = net.blobs[bottom_name].data[...]
    	    output_blob = net.blobs[layer].data[...]

    	    # Pad Convolution and Pooling Layers
    	    if parse_layer.type in {'Convolution', 'Pooling'}:
                if pad != 0:
                     pad_arr = np.zeros((input_blob.shape[0],
          		  input_blob.shape[1],
           		  input_blob.shape[2]+2*pad,
          	          input_blob.shape[3]+2*pad)).astype(np.float32)
        	     for i in range(0, input_blob.shape[0]):
          	         pad_arr[i,:,pad:-pad,pad:-pad] = input_blob[i]
      	        else:
                    pad_arr = input_blob

                layer_params['batch_size'] = pad_arr.shape[0]
                layer_params['input_dim'] = pad_arr.shape[1]
                layer_params['input_width'] = pad_arr.shape[2]
                layer_params['input_height'] = pad_arr.shape[3]
                layer_params['output_dim'] = output_blob.shape[1]
                layer_params['output_width'] = output_blob.shape[2]
                layer_params['output_height'] = output_blob.shape[3]
	    else:
      	        pad_arr = input_blob
                layer_params['batch_size'] = pad_arr.shape[0]
                layer_params['input_dim'] = pad_arr.shape[1]
                layer_params['output_dim'] = output_blob.shape[1]
	    	
	
	
    	    # Write inputs and outputs
    	    with open(dirname + '/input', 'wb') as f:
      	        pad_arr.tofile(f)
            with open(dirname + '/output', 'wb') as f:
                output_blob.tofile(f)
	

    	    # Create a packed version of inputs for DMA
    	    with open(dirname + '/dma_in', 'wb') as f:
    	        if W is not None: W.tofile(f)
    	        if b is not None: b.tofile(f)
    	        pad_arr.tofile(f)
    	    # Dump Parameters
            s = '\n'.join(["%s %s" % (k,str(v)) for k,v in layer_params.items()])
            with open(dirname + '/params', 'w') as f:
                f.write(s)
	num_batch = num_batch + 1
	    
    image_count += 1

