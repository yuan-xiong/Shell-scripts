#!/usr/bin/python

import os
import sys

import numpy as np
import caffe


def copy(org_net, dest_net):
    params_keys = dest_net.params.keys()
    for pr in params_keys:
	lidx = list(dest_net._layer_names).index(pr)
	oidx = list(org_net._layer_names).index(pr)
	layer = dest_net.layers[lidx]
	    # normal layer
        if layer.type == "Convolution" and len(dest_net.params[pr]) == 2:
            print "2 dest_net.params[pr] ", dest_net.params[pr]
            dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
            dest_net.layers[lidx].blobs[1].data.flat = org_net.layers[oidx].blobs[1].data.flat
        elif layer.type == "Convolution" and len(dest_net.params[pr]) == 1:
            print "1 dest_net.params[pr] ", dest_net.params[pr]
            dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
        elif layer.type == 'BatchNorm':
            dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
            dest_net.layers[lidx].blobs[1].data.flat = org_net.layers[oidx].blobs[1].data.flat
            dest_net.layers[lidx].blobs[2].data.flat = org_net.layers[oidx].blobs[2].data.flat
        elif layer.type == 'Scale':
            dest_net.layers[lidx].blobs[0].data.flat = org_net.layers[oidx].blobs[0].data.flat
            dest_net.layers[lidx].blobs[1].data.flat = org_net.layers[oidx].blobs[1].data.flat
        else:
            if len(dest_net.params[pr]) > 0:
                print 'missing !!!', layer.type

def generate_resnext101():
    # init org net
    net = caffe.Net(prototxt, weights, caffe.TEST)
    dest_net = caffe.Net("2-batchsize-16/resnext101_32x4d_features.prototxt-bak1221-batchsize-16",caffe.TEST)
    copy(net, dest_net)
    # export caffemodel
    dest_net.save("dummy_new_16_bit.caffemodel")

def generate_resnet50():
	print " ---> generate_resnet50"

def main():
	#generate_resnext101()
	generate_resnet50()
	return 0


if __name__ == "__main__":
    sys.exit(main())
