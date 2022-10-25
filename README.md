# A File Download Service Based on UDP
In this lab a UDP file download application was written in C. In this implementation. a simple data structure is imposed in the prodotocol data unit (PDU) exchanged between the client and server. The PDU has the following structure.

The data field contains data. The maximum size is 100 bytes. The "type" field is 1 byte. It specifies the PDU type. There are 4 PDU types. 

* Type = D --- DATA PDU
* Type = F --- FINAL PDU
* Type = E --- ERROR PDU
* TYPE = C --- FILENAME PDU

The DATA PDU is used by the server to send file data to the client. The data field contains the file data. The FINAL PDU carries the last batch of file data of a file and is also used to signal the end of a file download. The ERROR PDU is used to report an error. The FILENAME PDU is sent by the client to specify a file that the client wants to download from the server. 

Similar, to the File Download application using TCP. The client saves the downloaded data in the local file. On the other hand, error messages are displayed in the terminal.

In this implementation, I have used the lstate system call to obtain the file size; for the calculations required to find the last batch of data read from file.
