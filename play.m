clear

fid = fopen('sound.bin','rb'); 
wav=fread(fid,Inf,'uint8');
fclose(fid);
wav=(wav/255-0.5)*2;
sound(wav);
