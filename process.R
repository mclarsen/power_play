#!/usr/bin/env Rscript

d1 <- read.table("8nodes", header = T, sep = " ")
d1["speedup"] <- d1$old_time / d1$paviz_time
write.table(d1,
            file = "8nodes.tmp",
            append = FALSE, 
            sep = " ", 
            row.names = F, 
            col.names = T)

d2 <- read.table("64nodes", header = T, sep = " ")
d2["speedup"] <- d2$old_time / d2$paviz_time
write.table(d2,
            file = "64nodes.tmp",
            append = FALSE, 
            sep = " ", 
            row.names = F, 
            col.names = T)
