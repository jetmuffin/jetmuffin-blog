title: 文本挖掘初步 - 文本中文分词
tags: [文本挖掘,中文分词,ICTCLAS,NLP]
categories: 设计开发
date: 2015-06-05 22:32:00
---

## 介绍

中文分词(Chinese Word Segmentation)指的是将一个汉字序列切分成一个一个单独的词。

如同编译原理中的词法分析的重要性,中文分词是文本挖掘的基础,需要通过机器自动将输入的句子切分成一个个的词语,才能进行更多的处理.

中文分词和英文分词最大的区别在于,英文单词之间是以空格作为自然分界符的，而汉语是以字为基本的书写单位，词语之间没有明显的区分标记,因此中文分词的难度比英文分词大了很多.

## 方法

分词算法可分为三大类：基于字典、词库匹配的分词方法,基于词频度统计的分词方法和基于知识理解的分词方法。

1**. 基于字典,词库(字符串匹配)的分词方法**

这种方法按照一定策略将待分析的汉字串与一个“充分大的”机器词典中的词条进行匹配，若在词典中找到某个字符串，则匹配成功。识别出一个词，根据扫描方向的不同分为正向匹配和逆向匹配。采用该方法的分词算法主要有:

*   正向最大匹配法（由左到右的方向）
*   逆向最大匹配法（由右到左的方向）
*   最少切分（使每一句中切出的词数最小）

2.**基于词频度统计的分词方法**

这是一种全切分方法。它不依靠词典,而是将文章中任意两个字同时出现的频率进行统计,次数越高的就可能是一个词。它首先切分出与词表匹配的所有可能的词,运用统计语言模型和决策算法决定最优的切分结果。它的优点在于可以发现所有的切分歧义并且容易将新词提取出来。

3.**基于知识理解的分词**

该方法主要基于句法、语法分析，并结合语义分析，通过对上下文内容所提供信息的分析对词进行定界，它通常包括三个部分：分词子系统、句法语义子系统、总控部分。在总控部分的协调下，分词子系统可以获得有关词、句子等的句法和语义信息来对分词歧义进行判断。这类方法试图让机器具有人类的理解能力，需要使用大量的语言知识和信息。由于汉语语言知识的笼统、复杂性，难以将各种语言信息组织成机器可直接读取的形式。因此目前基于知识的分词系统还处在试验阶段。

## 工具

在中文分词领域,有很多已经非常成熟的分词工具可以被使用,以下对各工具的算法和效率(根据网上情报)进行总结.

1.**ICTCLAS(NLPIR)**

[http://ictclas.nlpir.org/](http://ictclas.nlpir.org/)

ICTCLAS是中国科学院计算技术研究所在多年研究基础上，耗时一年研制出了基于多层隐马模型(HMM)的汉语词法分析系统 ，功能有：中文分词；词性标注；自定义词典等。分词正确率高,分词速度快.源代码数以C++编写的,并提供JAVA接口.

速度: 单机 (996KB/s)

2.**Stanford Word Segmenter || CRF++**

[http://nlp.stanford.edu/software/segmenter.shtml](http://nlp.stanford.edu/software/segmenter.shtml)

这两个中文分词工具都是基于条件随机场CRF(<span lang="EN-US">C</span><span lang="EN-US">onditional<span class="Apple-converted-space"> </span></span><span lang="EN-US">R</span><span lang="EN-US">andom</span><span lang="EN-US"><span class="Apple-converted-space"> </span>F</span><span lang="EN-US">ield)模型实现的,<span lang="ZH-CN">其基本思路是对汉字进行标注即由字构词</span><span lang="EN-US">(</span><span lang="ZH-CN">组词</span><span lang="EN-US">)</span><span lang="ZH-CN">，不仅考虑了文字词语出现的频率信息，同时考虑上下文语境，具备较好的学习能力，因此其对歧义词和未登录词的识别都具有良好的效果；其不足之处是训练周期较长，运营时计算量较大.</span></span>

速度: (暂未获得)

3.**Mmseg **

[http://technology.chtsai.org/mmseg/](http://technology.chtsai.org/mmseg/)

Mmseg是典型的基于字典的中文分词工具,其主要实现原理是基于最大匹配算法.它提供Simple和Complex两种最大匹配算法,即简单的和复杂的.简单的最大匹配算法从字符串开头开始,获取最长的匹配串;而复杂的最大匹配算法则针对存在歧义的单词,向前再看两个单词再进行判断.

Mmseg支持在lucene和solr中使用,在该二者中配置Mmseg,即可对中文文本进行分词索引.这也是Mmseg被使用广泛的一个原因.同时因为其实现原理简单,效果也很不错,且提供Java支持(Mmseg4j),也是很受欢迎的一款分词工具.

速度: complex 60W字/s (1200 KB/s)  ；simple 100W字/s (1900 KB/s)

4.**IKAnalyzer**

[http://code.google.com/p/ik-analyzer/downloads/list](http://code.google.com/p/ik-analyzer/downloads/list)

IKAnalyzer是一个开源的，基于java语言开发的轻量级的中文分词工具包。采用了特有的"正向迭代最细粒度切分算法"(我还没取研究这个算法),支持细粒度和智能分词两种切分模式.同样,IKAnalyzer也提供了对Solr和Lucene的支持.

速度: IK2012 160W字/s (3000KB/s)

## 代码

根据分词的成功率,速率以及使用接口简单,在之后的所有后续挖掘过程中将采用ICTCLAS2015作为分词器,所以需要对其进行简单的封装.

JNA实现接口CLibrary:

```java
package com.jetmuffin.analyzer;
import com.sun.jna.Library;

public interface CLibrary extends Library{
    public int NLPIR_Init(String sDataPath, int encoding, String sLicenceCode);
    public String NLPIR_ParagraphProcess(String sSrc, int bPOSTagged);
    public String NLPIR_GetKeyWords(String sLine, int nMaxKeyLimit, boolean bWeightOut);
    public String NLPIR_GetFileKeyWords(String sLine, int nMaxKeyLimit, boolean bWeightOut);
    public int NLPIR_AddUserWord(String sWord);//add by qp 2008.11.10
    public int NLPIR_DelUsrWord(String sWord);//add by qp 2008.11.10
    public int NLPIR_ImportUserDict(String sWord,boolean bOverWrite);
    public String NLPIR_WordFreqStat(String sWord);
    public String NLPIR_GetLastErrorMsg();
    public void NLPIR_Exit();
}
```

    接口实现类Nlpir:

```java
package com.jetmuffin.analyzer;
import com.sun.jna.Native;

public class NLPIR {
    public static CLibrary Instance;
    private String systemCharset;
    private String systemFolder;
    int charsetType;

    public NLPIR(){
        systemFolder = "/home/jeff/src/ICTCLAS2015";  //ICTCLAS的目录,包括字典文件等
        systemCharset = "UTF-8";  //默认字符集
        charsetType = 1;

        Instance = (CLibrary) Native.loadLibrary(
                "/home/jeff/src/ICTCLAS2015/lib/linux32/libNLPIR.so", CLibrary.class);  //ICTCLAS的so文件地址
        }

        public static CLibrary getInstance() {
            return Instance;
        }

        public static void setInstance(CLibrary instance) {
            Instance = instance;
        }

        public String getSystemCharset() {
            return systemCharset;
        }

        public void setSystemCharset(String systemCharset) {
            this.systemCharset = systemCharset;
        }

        public String getSystemFolder() {
            return systemFolder;
        }

        public void setSystemFolder(String systemFolder) {
            this.systemFolder = systemFolder;
        }

        public int getCharsetType() {
            return charsetType;
        }

        public void setCharsetType(int charsetType) {
            this.charsetType = charsetType;
        }
    }
}
```

测试类:

```java
package com.jetmuffin.test;

import org.junit.Test;
import com.jetmuffin.NLPIR;

public class TestNlpir {
        @Test
    public void test(){
        NLPIR nlpir = new NLPIR();
        nlpir.Instance.NLPIR_Init(nlpir.getSystemFolder(),nlpir.getCharsetType() ,nlpir.getSystemCharset());
        String nativeBytes = null;

        String sInput = "什么是机器学习";
        nlpir.Instance.NLPIR_ImportUserDict("/home/jeff/workspace/data/dict", false);  //加载用户自定义词典
        nativeBytes = nlpir.Instance.NLPIR_ParagraphProcess(sInput, 0); //分词
        String[] words = nativeBytes.split(" ");  //分词结果以空格进行分割
        System.out.println(words.length); //输出分词词数
        System.out.println(nativeBytes);  //输出分词结果
        }
    }
}
```

结果:

```
3    
什么 是 机器学习 
```

## 结语

进行分类器的封装以后,可以继续进行之后的文本挖掘内容.以上均为本人拙见,且本人为NLP菜鸟,望大牛予以指正! :)