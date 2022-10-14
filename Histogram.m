function []=Histogram(histFileName)

    histFile = fopen(histFileName, "r");
    
    x = -32768:1:32767;
    y = fread(histFile, 65536, "uint64");
    y = permute(y, [2 1]);
    
    bar(x, y, 'BarWidth', 1);
    title("Histogram of " + histFileName);
    xlabel("Value (int16)");
    ylabel("Count");
    
    fclose(histFile);

end
