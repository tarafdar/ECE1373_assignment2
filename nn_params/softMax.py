__author__ = 'pittnuts'
'''
Main script to run classification/test/prediction/evaluation
'''
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import *
from PIL import Image
import caffe
import sys
import lmdb
from caffe.proto import caffe_pb2
from os import system
import argparse



def contains(X,v):
    X = np.array(X).flatten()
    for x in X:
        if x==v:
            return True

    return False



def contains2D(X,v):
    X = np.array(X)
    row_num = X.shape[0]
    res = np.zeros((row_num,1))
    for i in range(0,row_num):
        if contains(X[i,:],v[i]):
            res[i,0] = True
        else: res[i,0] = False
    return res





count=0
correct_top1=0
correct_top5=0


#model  = '/opt/caffe/models/bvlc_alexnet/deploy.prototxt'
#weights  = '/opt/caffe/models/bvlc_alexnet/bvlc_alexnet.caffemodel'
#image_root_path = '../../data/alexnet_batches' 
model  = '/opt/caffe/models/vgg16/VGG_ILSVRC_16_layers_deploy_update.prototxt'
weights  = '/opt/caffe/models/vgg16/VGG_ILSVRC_16_layers_update.caffemodel'
image_root_path = '../data/vgg_batches' 


caffe.set_mode_cpu()


net = caffe.Net(model, weights, caffe.TEST)
batch_size = net.blobs['data'].data[...].shape[0]


total_batches=50
last_fc = 'fc8'
prob = 'prob'

for i in range(0, total_batches):
    last_fc_path = image_root_path + '/batch_'+ str(i) + '/' + last_fc + '/'
    last_fc_out = last_fc_path + 'output'
    label_path = image_root_path + '/batch_'+ str(i) + '/labels' 
    fc_input = np.fromfile(last_fc_out, dtype=np.float32)
    labels = np.fromfile(label_path, dtype=np.float64)
    #print labels
    fc_input = fc_input.reshape(batch_size, len(fc_input)/batch_size)
    labels = labels.reshape(batch_size, len(labels)/batch_size)
    #print fc_input
    net.blobs[last_fc].data[...] = fc_input 
    out = net.forward(start=prob)
    plabel = out[prob][:].argmax(axis=1)
    #print plabel
    correct_top1 = correct_top1 + sum(labels.flatten() == plabel.flatten())
    plabel_top5 = np.argsort(out['prob'][:],axis=1)[:,-1:-6:-1]
    correct_top5_count = sum(contains2D(plabel_top5,labels))
    correct_top5 = correct_top5 + correct_top5_count    
    #print correct_top1


print correct_top1
print correct_top5
