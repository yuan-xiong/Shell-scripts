FROM ubuntu:bionic

#ARG DEBIAN_FRONTEND=noninteractive

#RUN apt-get -q -y update \
#        && apt-get install -y vim

#COPY test.txt /test.txt

#COPY src/a.out /a.out
COPY src/mknod /mknod
COPY src/check-dev.sh /check-dev.sh

#COPY lsmod.sh /lsmod.sh

ADD dev-xy/lsmod.sh /lsmod.sh

#CMD echo "welcome back, xy!"
#CMD ls -l /

#CMD lsmod 
#CMD /bin/bash lsmod 
#CMD ["sh", "lsmod.sh"]

#ENTRYPOINT ["/aic-manager-entry"]
#ENTRYPOINT ["/a.out"]
#ENTRYPOINT ["/lsmod.sh"]
#
#ENTRYPOINT ["/mknod"]

