// all helper functions

// get cart number from array data NFC
char *Get_Card_Number( int num[] ){
  memset(outNumberCart, 0, 255);
  int i = 0;
  while (i<=15 && num[i]==0)
    i++;
  
  while (i<=15) {
    if(num[i]<=9){
        if(strlen(outNumberCart) != 0)
          strcat(outNumberCart, "0");
      char char_arr [100];
      sprintf(char_arr, "%d", num[i]);
      strcat(outNumberCart, char_arr);
    }else{
      char char_arr [100];
      sprintf(char_arr, "%d", num[i]);
      strcat(outNumberCart, char_arr);
    }
    i++;
  }
  return outNumberCart;
}

