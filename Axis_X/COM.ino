void odesliData(int komu, int typ_dat, long data_1, long data_2) {
  et_tx_data.ktery_uzel = komu; // ktere ose
  et_tx_data.typ_dat = typ_dat; // co odesílám
  et_tx_data.data_1 = data_1;
  et_tx_data.data_2 = data_2;

  ET_Tx.sendData();
}

