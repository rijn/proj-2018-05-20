rm -rf ./temp
mkdir temp

cd stage_1
clang++ inputo.cpp AP.cpp testo.cpp -o execute
cd ..
mv ./stage_1/execute ./temp/run_stage_1

cd stage_2
clang++ GBDME.cpp test2.cpp -o execute
cd ..
mv ./stage_2/execute ./temp/run_stage_2

cp -r ispd2009 temp/ispd2009
cp -r bufferInsertion temp/bufferInsertion

cd temp

./run_stage_1
./run_stage_2