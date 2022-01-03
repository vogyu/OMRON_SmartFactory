install.packages("dplyr")
library(dplyr)
library(ggplot2)
## readLines() = char ///////  read.table() = int
pixel <- read.table("data/pixel.txt")
pixel <- as.data.frame(pixel)
##str(pixel)
pixel <- pixel %>% 
  mutate(count = ifelse(pixel >= 40000, "color pet",
                        ifelse(pixel >= 11000,"label clear PET","no label clear PET")))

table(pixel$count)
ggplot(data=pixel,aes(x=count,fill=count)) + xlab("") + ylab("") + 
  geom_bar() + theme(plot.title = element_text(hjust = 0.5,size = 50, vjust = -0.5),
                     plot.subtitle = element_text(color = "blue"),
                     legend.position = "none", 
                     axis.title.x = element_text(size = 15),
                     axis.title.y = element_text(size = 15),
                     axis.text.x = element_text(size = 15),
                     axis.text.y = element_text(size = 15)) + labs(title = "페트병 분류 현황", subtitle = "OMRON & KPU")

head(pixel$count, 30)
head(pixel,30)

