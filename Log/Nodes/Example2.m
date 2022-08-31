clear all
close all

agg_eng_nom = struct2cell(load('agg_egg_nom.mat'));
agg_time_nom = struct2cell(load('agg_time_nom.mat'));
cp_energy_nom = struct2cell(load('cp_energy_nom.mat'));
cp_time_nom = struct2cell(load('cp_time_norm.mat'));
st_energy_nom = struct2cell(load('st_energy_norm.mat'));
st_time_nom = struct2cell(load('st_time_norm.mat'));

k = 0.8;
q = 1-k;

agg = k .* agg_eng_nom{1,:} + q .* agg_time_nom{1,:};

figure
loglog([1, 2, 5, 8, 10, 20, 50, 80, 100]./10, agg);

cp = k .* cp_energy_nom{1,:} + q .* cp_time_nom{1,:};

figure
loglog([1000, 2000, 5000, 8000, 10000, 20000, 50000, 80000, 100000]./10000, cp);


figure
st = k .* st_energy_nom{1,:} + q .* st_time_nom{1,:};
loglog([100, 200, 500, 625, 1000, 2000, 2500, 5000 10000]./1000, st);
