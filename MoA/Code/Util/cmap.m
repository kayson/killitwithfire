function cmap(map)
% Simple script to print out Matlab colormaps in a format
% suitable for storing in a c++ std::vector<vec3f>
s = size(map);
for i=1:s(1,1)
    fprintf(1, 'mColors.push_back(Vector3<float>(%f, %f, %f));\n', map(i,1), map(i,2), map(i,3));
end
