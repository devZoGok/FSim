Paruošimas:
Įsirašyti „mysql++“ ir „boost“
Sukurti MariaDB/MySQL vartotoją su visomis privilegijomis.
Slaptažodis turi būti tuščias.
Sukurti duomenų bazę „fsim“.
Faile „options.cfg“ paskutinės eilutės reikšmę pakeisti duomenų bazės
vartotojo pavadinimu.
cmake -S . -B build
cd build
make
