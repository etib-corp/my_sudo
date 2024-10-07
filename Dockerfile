FROM fedora:latest

# Install dependencies make, gcc and libcrypt
RUN dnf install -y make gcc libxcrypt-compat

# install gdb
RUN dnf install -y gdb

WORKDIR /app

# Copy the source code
COPY . /app

# create a user name it toto
RUN useradd -ms /bin/bash toto

# change the password of the user toto
RUN echo 'toto:s3s4m3' | chpasswd

# ADD toto to the sudoers file
RUN echo "toto ALL=(ALL:ALL) NOPASSWD: ALL" >> /etc/sudoers

# add toto to the groupe wheel
RUN  usermod -a -G wheel toto

# create a user name it tata
RUN useradd -ms /bin/bash tata

# change the password of the user tatat
RUN echo 'tata:s3s4m3' | chpasswd

# ADD tata to the sudoers file
RUN echo "tata ALL=(ALL:ALL) NOPASSWD: ALL" >> /etc/sudoers

# add tata to the groupe wheel
RUN  usermod -a -G wheel tata

# Change the owner of the source code to toto
RUN chown -R tata:tata /app

# Change the user to toto
USER toto

# Compile the source code
RUN make

RUN sudo chown root:root my_sudo

# Run the compiled code
# CMD ["./mysudo", "/bin/cat", "main.c"]
