import numpy as np
import matplotlib.pyplot as plt
from scipy.io import *
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


model  = '/opt/caffe/models/vgg16/VGG_ILSVRC_16_layers_deploy_update.prototxt'
weights  = '/opt/caffe/models/vgg16/VGG_ILSVRC_16_layers_update.caffemodel'
image_root_path = '../data/vgg_batches' 
total_batches=10
last_fc = 'fc8'
prob = 'prob'

parser = argparse.ArgumentParser()
parser.add_argument('--model', action='store', dest='model', default=model, type= str)
parser.add_argument('--weights', action='store', dest='weights', default=weights, type= str)
parser.add_argument('--image_root_path', action='store', dest='image_root_path', default=image_root_path, type= str)
parser.add_argument('--total_batches', action='store', dest='total_batches', default=total_batches, type= int)
parser.add_argument('--last_fc', action='store', dest='last_fc', default=last_fc, type= str)
parser.add_argument('--softmax', action='store', dest='prob', default=prob, type= str)

results = parser.parse_args()
model = results.model
weights = results.weights
image_root_path = results.image_root_path
total_batches = results.total_batches
last_fc = results.last_fc
prob = results.prob

caffe.set_mode_cpu()


net = caffe.Net(model, weights, caffe.TEST)
batch_size = net.blobs['data'].data[...].shape[0]

print "Batch Size " + str(batch_size)
print "Processing  " + str(total_batches) + " batches"

for i in range(0, total_batches):
    print "Processing Batch " + str(i)
    last_fc_path = image_root_path + '/batch_'+ str(i) + '/' + last_fc + '/'
    last_fc_out = last_fc_path + 'dma_out'
    label_path = image_root_path + '/batch_'+ str(i) + '/labels' 
    fc_input = np.fromfile(last_fc_out, dtype=np.float32)
    labels = np.fromfile(label_path, dtype=np.float64)
    fc_input = fc_input.reshape(batch_size, len(fc_input)/batch_size)
    labels = labels.reshape(batch_size, len(labels)/batch_size)
    net.blobs[last_fc].data[...] = fc_input 
    out = net.forward(start=prob)
    plabel = out[prob][:].argmax(axis=1)
    correct_top1 = correct_top1 + sum(labels.flatten() == plabel.flatten())
    plabel_top5 = np.argsort(out['prob'][:],axis=1)[:,-1:-6:-1]
    correct_top5_count = sum(contains2D(plabel_top5,labels))
    correct_top5 = correct_top5 + correct_top5_count    
    
correct_top1_pct = 100.0 * float(correct_top1)/float(total_batches * batch_size)
correct_top5_pct = 100.0 * float(correct_top5)/float(total_batches * batch_size)

print "Top 1 percent correct " + str(correct_top1_pct)
print "Top 5 percent correct " + str(correct_top5_pct)
