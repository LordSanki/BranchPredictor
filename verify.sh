./sim bimodal 7 0 0 gcc_trace.txt > vb1.txt
./sim bimodal 10 0 0 gcc_trace.txt > vb2.txt
./sim bimodal 5 0 0 jpeg_trace.txt > vb3.txt
./sim bimodal 6 0 0 perl_trace.txt > vb4.txt
./sim gshare 10 4 0 0 gcc_trace.txt > vg1.txt
./sim gshare 14 9 0 0 gcc_trace.txt > vg2.txt
./sim gshare 11 5 0 0 jpeg_trace.txt > vg3.txt
./sim gshare 10 6 0 0 perl_trace.txt > vg4.txt
./sim hybrid 9 14 11 6 0 0 gcc_trace.txt >vh1.txt
./sim hybrid 5 10 8 5 0 0 jpeg_trace.txt >vh2.txt


diff -iyw --suppress-common-lines vb1.txt val_bimodal_1.txt
diff -iyw --suppress-common-lines vb2.txt val_bimodal_2.txt
diff -iyw --suppress-common-lines vb3.txt val_bimodal_3.txt
diff -iyw --suppress-common-lines vb4.txt val_bimodal_4.txt
diff -iyw --suppress-common-lines vg1.txt val_gshare_1.txt
diff -iyw --suppress-common-lines vg2.txt val_gshare_2.txt
diff -iyw --suppress-common-lines vg3.txt val_gshare_3.txt
diff -iyw --suppress-common-lines vg4.txt val_gshare_4.txt
diff -iyw --suppress-common-lines vh1.txt val_hybrid_1.txt
diff -iyw --suppress-common-lines vh2.txt val_hybrid_2.txt

rm -f vb1.txt vb2.txt vb3.txt vb4.txt vg1.txt vg2.txt vg3.txt vg4.txt vh1.txt vh2.txt
