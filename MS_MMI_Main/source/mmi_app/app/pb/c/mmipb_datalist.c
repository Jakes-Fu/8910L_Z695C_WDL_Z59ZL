/****************************************************************************
** File Name:      mmipb_datalist.c                                        *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with the contact list and qsort list      *             
**                 这个文件对记录索引表，快速查找表的管理.  
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_DATALIST_C_  


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mn_type.h"
#include "mmipb_datalist.h"
#include "mmipb_storage.h"
#include "mmipb_interface.h"
#include "mmifdn_export.h"
#include "mmiim.h"
#include "mmipb_search.h"


#ifdef MMIPB_SYNC_WITH_O365
#include "mmipb_sync_datalist.h"
#include "mmipb_sync_storage.h"
#include "mmisrv_contactsync_datalist.h"

#include "mmipb_task.h"
#include "mmipb_text.h"
#include "mmipb_set.h"
#include "mmipb_view.h"

#include "mmimsa_export.h"
#include "mmiconnection_export.h"
#include "mmisrv_contactsync_http.h" 

//#define MMIPB_SYNC_HEART_BEAT_TIMEOUT       2000
#define MMIPB_SYNC_GET_COUNT_PARALLEL

//#define MMIPB_SYNC_USE_PC_TOKEN //shiwei add test
#include "mmipb_app.h"
#include "mmi_appmsg.h"

#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPB_MASK_ENTRY_ID_FLAG_1              0X01

#define MMIPB_INVALID_INDEX                     0xFFFF

#define MMIPB_EMPTY_STRING_FLAG                 0xFFFF
#define MMIPB_KEY_HEAD_FLAG                     0X8000//最高为1表示是键值，否则为字符
#define MMIPB_CONTACT_INDEX_MASK                0X7FFF//取联系人索引


#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
#define HANZI_START 19968
#define HANZI_COUNT 20902
static const char firstLetterArray[HANZI_COUNT+1] = 
"ydkqsxnwzssxjbymgcczqpssqbycdscdqldylybssjgyqzjjfgcclzznwdwzjljpfyynnjjtmynzwzhflzppqhgccyynmjqyxxgd"
"nnsnsjnjnsnnmlnrxyfsngnnnnqzggllyjlnyzssecykyyhqwjssggyxyqyjtwktjhychmnxjtlhjyqbyxdldwrrjnwysrldzjpc"
"bzjjbrcfslnczstzfxxchtrqggddlyccssymmrjcyqzpwwjjyfcrwfdfzqpyddwyxkyjawjffxjbcftzyhhycyswccyxsclcxxwz"
"cxnbgnnxbxlzsqsbsjpysazdhmdzbqbscwdzzyytzhbtsyyfzgntnxjywqnknphhlxgybfmjnbjhhgqtjcysxstkzglyckglysmz"
"xyalmeldccxgzyrjxjzlnjzcqkcnnjwhjczccqljststbnhbtyxceqxkkwjyflzqlyhjxspsfxlmpbysxxxytccnylllsjxfhjxp"
"jbtffyabyxbcczbzyclwlczggbtssmdtjcxpthyqtgjjxcjfzkjzjqnlzwlslhdzbwjncjzyzsqnycqynzcjjwybrtwpyftwexcs"
"kdzctbyhyzqyyjxzcfbzzmjyxxsdczottbzljwfckscsxfyrlrygmbdthjxsqjccsbxyytswfbjdztnbcnzlcyzzpsacyzzsqqcs"
"hzqydxlbpjllmqxqydzxsqjtzpxlcglqdcwzfhctdjjsfxjejjtlbgxsxjmyjjqpfzasyjnsydjxkjcdjsznbartcclnjqmwnqnc"
"lllkbdbzzsyhqcltwlccrshllzntylnewyzyxczxxgdkdmtcedejtsyyssdqdfmxdbjlkrwnqlybglxnlgtgxbqjdznyjsjyjcjm"
"rnymgrcjczgjmzmgxmmryxkjnymsgmzzymknfxmbdtgfbhcjhkylpfmdxlxjjsmsqgzsjlqdldgjycalcmzcsdjllnxdjffffjcn"
"fnnffpfkhkgdpqxktacjdhhzdddrrcfqyjkqccwjdxhwjlyllzgcfcqjsmlzpbjjblsbcjggdckkdezsqcckjgcgkdjtjllzycxk"
"lqccgjcltfpcqczgwbjdqyzjjbyjhsjddwgfsjgzkcjctllfspkjgqjhzzljplgjgjjthjjyjzccmlzlyqbgjwmljkxzdznjqsyz"
"mljlljkywxmkjlhskjhbmclyymkxjqlbmllkmdxxkwyxwslmlpsjqqjqxyqfjtjdxmxxllcrqbsyjbgwynnggbcnxpjtgpapfgdj"
"qbhbncfjyzjkjkhxqfgqckfhygkhdkllsdjqxpqyaybnqsxqnszswhbsxwhxwbzzxdmndjbsbkbbzklylxgwxjjwaqzmywsjqlsj"
"xxjqwjeqxnchetlzalyyyszzpnkyzcptlshtzcfycyxyljsdcjqagyslcllyyysslqqqnldxzsccscadycjysfsgbfrsszqsbxjp"
"sjysdrckgjlgtkzjzbdktcsyqpyhstcldjnhmymcgxyzhjdctmhltxzhylamoxyjcltyfbqqjpfbdfehthsqhzywwcncxcdwhowg"
"yjlegmdqcwgfjhcsntmydolbygnqwesqpwnmlrydzszzlyqpzgcwxhnxpyxshmdqjgztdppbfbhzhhjyfdzwkgkzbldnzsxhqeeg"
"zxylzmmzyjzgszxkhkhtxexxgylyapsthxdwhzydpxagkydxbhnhnkdnjnmyhylpmgecslnzhkxxlbzzlbmlsfbhhgsgyyggbhsc"
"yajtxglxtzmcwzydqdqmngdnllszhngjzwfyhqswscelqajynytlsxthaznkzzsdhlaxxtwwcjhqqtddwzbcchyqzflxpslzqgpz"
"sznglydqtbdlxntctajdkywnsyzljhhdzckryyzywmhychhhxhjkzwsxhdnxlyscqydpslyzwmypnkxyjlkchtyhaxqsyshxasmc"
"hkdscrsgjpwqsgzjlwwschsjhsqnhnsngndantbaalczmsstdqjcjktscjnxplggxhhgoxzcxpdmmhldgtybynjmxhmrzplxjzck"
"zxshflqxxcdhxwzpckczcdytcjyxqhlxdhypjqxnlsyydzozjnhhqezysjyayxkypdgxddnsppyzndhthrhxydpcjjhtcnnctlhb"
"ynyhmhzllnnxmylllmdcppxhmxdkycyrdltxjchhznxclcclylnzsxnjzzlnnnnwhyqsnjhxynttdkyjpychhyegkcwtwlgjrlgg"
"tgtygyhpyhylqyqgcwyqkpyyettttlhyylltyttsylnyzwgywgpydqqzzdqnnkcqnmjjzzbxtqfjkdffbtkhzkbxdjjkdjjtlbwf"
"zpptkqtztgpdwntpjyfalqmkgxbcclzfhzcllllanpnxtjklcclgyhdzfgyddgcyyfgydxkssendhykdndknnaxxhbpbyyhxccga"
"pfqyjjdmlxcsjzllpcnbsxgjyndybwjspcwjlzkzddtacsbkzdyzypjzqsjnkktknjdjgyepgtlnyqnacdntcyhblgdzhbbydmjr"
"egkzyheyybjmcdtafzjzhgcjnlghldwxjjkytcyksssmtwcttqzlpbszdtwcxgzagyktywxlnlcpbclloqmmzsslcmbjcsdzkydc"
"zjgqjdsmcytzqqlnzqzxssbpkdfqmddzzsddtdmfhtdycnaqjqkypbdjyyxtljhdrqxlmhkydhrnlklytwhllrllrcxylbnsrnzz"
"symqzzhhkyhxksmzsyzgcxfbnbsqlfzxxnnxkxwymsddyqnggqmmyhcdzttfgyyhgsbttybykjdnkyjbelhdypjqnfxfdnkzhqks"
"byjtzbxhfdsbdaswpawajldyjsfhblcnndnqjtjnchxfjsrfwhzfmdrfjyxwzpdjkzyjympcyznynxfbytfyfwygdbnzzzdnytxz"
"emmqbsqehxfznbmflzzsrsyqjgsxwzjsprytjsjgskjjgljjynzjjxhgjkymlpyyycxycgqzswhwlyrjlpxslcxmnsmwklcdnkny"
"npsjszhdzeptxmwywxyysywlxjqcqxzdclaeelmcpjpclwbxsqhfwrtfnjtnqjhjqdxhwlbyccfjlylkyynldxnhycstyywncjtx"
"ywtrmdrqnwqcmfjdxzmhmayxnwmyzqtxtlmrspwwjhanbxtgzypxyyrrclmpamgkqjszycymyjsnxtplnbappypylxmyzkynldgy"
"jzcchnlmzhhanqnbgwqtzmxxmllhgdzxnhxhrxycjmffxywcfsbssqlhnndycannmtcjcypnxnytycnnymnmsxndlylysljnlxys"
"sqmllyzlzjjjkyzzcsfbzxxmstbjgnxnchlsnmcjscyznfzlxbrnnnylmnrtgzqysatswryhyjzmgdhzgzdwybsscskxsyhytsxg"
"cqgxzzbhyxjscrhmkkbsczjyjymkqhzjfnbhmqhysnjnzybknqmcjgqhwlsnzswxkhljhyybqcbfcdsxdldspfzfskjjzwzxsddx"
"jseeegjscssygclxxnwwyllymwwwgydkzjggggggsycknjwnjpcxbjjtqtjwdsspjxcxnzxnmelptfsxtllxcljxjjljsxctnswx"
"lennlyqrwhsycsqnybyaywjejqfwqcqqcjqgxaldbzzyjgkgxbltqyfxjltpydkyqhpmatlcndnkxmtxynhklefxdllegqtymsaw"
"hzmljtkynxlyjzljeeyybqqffnlyxhdsctgjhxywlkllxqkcctnhjlqmkkzgcyygllljdcgydhzwypysjbzjdzgyzzhywyfqdtyz"
"szyezklymgjjhtsmqwyzljyywzcsrkqyqltdxwcdrjalwsqzwbdcqyncjnnszjlncdcdtlzzzacqqzzddxyblxcbqjylzllljddz"
"jgyqyjzyxnyyyexjxksdaznyrdlzyyynjlslldyxjcykywnqcclddnyyynycgczhjxcclgzqjgnwnncqqjysbzzxyjxjnxjfzbsb"
"dsfnsfpzxhdwztdmpptflzzbzdmyypqjrsdzsqzsqxbdgcpzswdwcsqzgmdhzxmwwfybpngphdmjthzsmmbgzmbzjcfzhfcbbnmq"
"dfmbcmcjxlgpnjbbxgyhyyjgptzgzmqbqdcgybjxlwnkydpdymgcftpfxyztzxdzxtgkptybbclbjaskytssqyymscxfjhhlslls"
"jpqjjqaklyldlycctsxmcwfgngbqxllllnyxtyltyxytdpjhnhgnkbyqnfjyyzbyyessessgdyhfhwtcqbsdzjtfdmxhcnjzymqw"
"srxjdzjqbdqbbsdjgnfbknbxdkqhmkwjjjgdllthzhhyyyyhhsxztyyyccbdbpypzyccztjpzywcbdlfwzcwjdxxhyhlhwczxjtc"
"nlcdpxnqczczlyxjjcjbhfxwpywxzpcdzzbdccjwjhmlxbqxxbylrddgjrrctttgqdczwmxfytmmzcwjwxyywzzkybzcccttqnhx"
"nwxxkhkfhtswoccjybcmpzzykbnnzpbthhjdlszddytyfjpxyngfxbyqxzbhxcpxxtnzdnnycnxsxlhkmzxlthdhkghxxsshqyhh"
"cjyxglhzxcxnhekdtgqxqypkdhentykcnymyyjmkqyyyjxzlthhqtbyqhxbmyhsqckwwyllhcyylnneqxqwmcfbdccmljggxdqkt"
"lxkknqcdgcjwyjjlyhhqyttnwchhxcxwherzjydjccdbqcdgdnyxzdhcqrxcbhztqcbxwgqwyybxhmbymykdyecmqkyaqyngyzsl"
"fnkkqgyssqyshngjctxkzycssbkyxhyylstycxqthysmnscpmmgcccccmnztasmgqzjhklosjylswtmqzyqkdzljqqyplzycztcq"
"qpbbcjzclpkhqcyyxxdtdddsjcxffllchqxmjlwcjcxtspycxndtjshjwhdqqqckxyamylsjhmlalygxcyydmamdqmlmcznnyybz"
"xkyflmcncmlhxrcjjhsylnmtjggzgywjxsrxcwjgjqhqzdqjdcjjskjkgdzcgjjyjylxzxxcdqhhheslmhlfsbdjsyyshfyssczq"
"lpbdrfnztzdkykhsccgkwtqzckmsynbcrxqbjyfaxpzzedzcjykbcjwhyjbqzzywnyszptdkzpfpbaztklqnhbbzptpptyzzybhn"
"ydcpzmmcycqmcjfzzdcmnlfpbplngqjtbttajzpzbbdnjkljqylnbzqhksjznggqstzkcxchpzsnbcgzkddzqanzgjkdrtlzldwj"
"njzlywtxndjzjhxnatncbgtzcsskmljpjytsnwxcfjwjjtkhtzplbhsnjssyjbhbjyzlstlsbjhdnwqpslmmfbjdwajyzccjtbnn"
"nzwxxcdslqgdsdpdzgjtqqpsqlyyjzlgyhsdlctcbjtktyczjtqkbsjlgnnzdncsgpynjzjjyyknhrpwszxmtncszzyshbyhyzax"
"ywkcjtllckjjtjhgcssxyqyczbynnlwqcglzgjgqyqcczssbcrbcskydznxjsqgxssjmecnstjtpbdlthzwxqwqczexnqczgwesg"
"ssbybstscslccgbfsdqnzlccglllzghzcthcnmjgyzazcmsksstzmmzckbjygqljyjppldxrkzyxccsnhshhdznlzhzjjcddcbcj"
"xlbfqbczztpqdnnxljcthqzjgylklszzpcjdscqjhjqkdxgpbajynnsmjtzdxlcjyryynhjbngzjkmjxltbsllrzpylssznxjhll"
"hyllqqzqlsymrcncxsljmlzltzldwdjjllnzggqxppskyggggbfzbdkmwggcxmcgdxjmcjsdycabxjdlnbcddygskydqdxdjjyxh"
"saqazdzfslqxxjnqzylblxxwxqqzbjzlfbblylwdsljhxjyzjwtdjcyfqzqzzdzsxzzqlzcdzfxhwspynpqzmlpplffxjjnzzyls"
"jnyqzfpfzgsywjjjhrdjzzxtxxglghtdxcskyswmmtcwybazbjkshfhgcxmhfqhyxxyzftsjyzbxyxpzlchmzmbxhzzssyfdmncw"
"dabazlxktcshhxkxjjzjsthygxsxyyhhhjwxkzxssbzzwhhhcwtzzzpjxsyxqqjgzyzawllcwxznxgyxyhfmkhydwsqmnjnaycys"
"pmjkgwcqhylajgmzxhmmcnzhbhxclxdjpltxyjkdyylttxfqzhyxxsjbjnayrsmxyplckdnyhlxrlnllstycyyqygzhhsccsmcct"
"zcxhyqfpyyrpbflfqnntszlljmhwtcjqyzwtlnmlmdwmbzzsnzrbpdddlqjjbxtcsnzqqygwcsxfwzlxccrszdzmcyggdyqsgtnn"
"nlsmymmsyhfbjdgyxccpshxczcsbsjyygjmpbwaffyfnxhydxzylremzgzzyndsznlljcsqfnxxkptxzgxjjgbmyyssnbtylbnlh"
"bfzdcyfbmgqrrmzszxysjtznnydzzcdgnjafjbdknzblczszpsgcycjszlmnrznbzzldlnllysxsqzqlcxzlsgkbrxbrbzcycxzj"
"zeeyfgklzlnyhgzcgzlfjhgtgwkraajyzkzqtsshjjxdzyznynnzyrzdqqhgjzxsszbtkjbbfrtjxllfqwjgclqtymblpzdxtzag"
"bdhzzrbgjhwnjtjxlkscfsmwlldcysjtxkzscfwjlbnntzlljzllqblcqmqqcgcdfpbphzczjlpyyghdtgwdxfczqyyyqysrclqz"
"fklzzzgffcqnwglhjycjjczlqzzyjbjzzbpdcsnnjgxdqnknlznnnnpsntsdyfwwdjzjysxyyczcyhzwbbyhxrylybhkjksfxtjj"
"mmchhlltnyymsxxyzpdjjycsycwmdjjkqyrhllngpngtlyycljnnnxjyzfnmlrgjjtyzbsyzmsjyjhgfzqmsyxrszcytlrtqzsst"
"kxgqkgsptgxdnjsgcqcqhmxggztqydjjznlbznxqlhyqgggthqscbyhjhhkyygkggcmjdzllcclxqsftgjslllmlcskctbljszsz"
"mmnytpzsxqhjcnnqnyexzqzcpshkzzyzxxdfgmwqrllqxrfztlystctmjcsjjthjnxtnrztzfqrhcgllgcnnnnjdnlnnytsjtlny"
"xsszxcgjzyqpylfhdjsbbdczgjjjqzjqdybssllcmyttmqnbhjqmnygjyeqyqmzgcjkpdcnmyzgqllslnclmholzgdylfzslncnz"
"lylzcjeshnyllnxnjxlyjyyyxnbcljsswcqqnnyllzldjnllzllbnylnqchxyyqoxccqkyjxxxyklksxeyqhcqkkkkcsnyxxyqxy"
"gwtjohthxpxxhsnlcykychzzcbwqbbwjqcscszsslcylgddsjzmmymcytsdsxxscjpqqsqylyfzychdjynywcbtjsydchcyddjlb"
"djjsodzyqyskkyxdhhgqjyohdyxwgmmmazdybbbppbcmnnpnjzsmtxerxjmhqdntpjdcbsnmssythjtslmltrcplzszmlqdsdmjm"
"qpnqdxcfrnnfsdqqyxhyaykqyddlqyyysszbydslntfgtzqbzmchdhczcwfdxtmqqsphqwwxsrgjcwnntzcqmgwqjrjhtqjbbgwz"
"fxjhnqfxxqywyyhyscdydhhqmrmtmwctbszppzzglmzfollcfwhmmsjzttdhlmyffytzzgzyskjjxqyjzqbhmbzclyghgfmshpcf"
"zsnclpbqsnjyzslxxfpmtyjygbxlldlxpzjyzjyhhzcywhjylsjexfszzywxkzjlnadymlymqjpwxxhxsktqjezrpxxzghmhwqpw"
"qlyjjqjjzszcnhjlchhnxjlqwzjhbmzyxbdhhypylhlhlgfwlcfyytlhjjcwmscpxstkpnhjxsntyxxtestjctlsslstdlllwwyh"
"dnrjzsfgxssyczykwhtdhwjglhtzdqdjzxxqgghltzphcsqfclnjtclzpfstpdynylgmjllycqhynspchylhqyqtmzymbywrfqyk"
"jsyslzdnjmpxyyssrhzjnyqtqdfzbwwdwwrxcwggyhxmkmyyyhmxmzhnksepmlqqmtcwctmxmxjpjjhfxyyzsjzhtybmstsyjznq"
"jnytlhynbyqclcycnzwsmylknjxlggnnpjgtysylymzskttwlgsmzsylmpwlcwxwqcssyzsyxyrhssntsrwpccpwcmhdhhxzdzyf"
"jhgzttsbjhgyglzysmyclllxbtyxhbbzjkssdmalhhycfygmqypjyjqxjllljgclzgqlycjcctotyxmtmshllwlqfxymzmklpszz"
"cxhkjyclctyjcyhxsgyxnnxlzwpyjpxhjwpjpwxqqxlxsdhmrslzzydwdtcxknstzshbsccstplwsscjchjlcgchssphylhfhhxj"
"sxallnylmzdhzxylsxlmzykcldyahlcmddyspjtqjzlngjfsjshctsdszlblmssmnyymjqbjhrzwtyydchjljapzwbgqxbkfnbjd"
"llllyylsjydwhxpsbcmljpscgbhxlqhyrljxyswxhhzlldfhlnnymjljyflyjycdrjlfsyzfsllcqyqfgqyhnszlylmdtdjcnhbz"
"llnwlqxygyyhbmgdhxxnhlzzjzxczzzcyqzfngwpylcpkpykpmclgkdgxzgxwqbdxzzkzfbddlzxjtpjpttbythzzdwslcpnhslt"
"jxxqlhyxxxywzyswttzkhlxzxzpyhgzhknfsyhntjrnxfjcpjztwhplshfcrhnslxxjxxyhzqdxqwnnhyhmjdbflkhcxcwhjfyjc"
"fpqcxqxzyyyjygrpynscsnnnnchkzdyhflxxhjjbyzwttxnncyjjymswyxqrmhxzwfqsylznggbhyxnnbwttcsybhxxwxyhhxyxn"
"knyxmlywrnnqlxbbcljsylfsytjzyhyzawlhorjmnsczjxxxyxchcyqryxqzddsjfslyltsffyxlmtyjmnnyyyxltzcsxqclhzxl"
"wyxzhnnlrxkxjcdyhlbrlmbrdlaxksnlljlyxxlynrylcjtgncmtlzllcyzlpzpzyawnjjfybdyyzsepckzzqdqpbpsjpdyttbdb"
"bbyndycncpjmtmlrmfmmrwyfbsjgygsmdqqqztxmkqwgxllpjgzbqrdjjjfpkjkcxbljmswldtsjxldlppbxcwkcqqbfqbccajzg"
"mykbhyhhzykndqzybpjnspxthlfpnsygyjdbgxnhhjhzjhstrstldxskzysybmxjlxyslbzyslzxjhfybqnbylljqkygzmcyzzym"
"ccslnlhzhwfwyxzmwyxtynxjhbyymcysbmhysmydyshnyzchmjjmzcaahcbjbbhblytylsxsnxgjdhkxxtxxnbhnmlngsltxmrhn"
"lxqqxmzllyswqgdlbjhdcgjyqyymhwfmjybbbyjyjwjmdpwhxqldyapdfxxbcgjspckrssyzjmslbzzjfljjjlgxzgyxyxlszqkx"
"bexyxhgcxbpndyhwectwwcjmbtxchxyqqllxflyxlljlssnwdbzcmyjclwswdczpchqekcqbwlcgydblqppqzqfnqdjhymmcxtxd"
"rmzwrhxcjzylqxdyynhyyhrslnrsywwjjymtltllgtqcjzyabtckzcjyccqlysqxalmzynywlwdnzxqdllqshgpjfjljnjabcqzd"
"jgthhsstnyjfbswzlxjxrhgldlzrlzqzgsllllzlymxxgdzhgbdphzpbrlwnjqbpfdwonnnhlypcnjccndmbcpbzzncyqxldomzb"
"lzwpdwyygdstthcsqsccrsssyslfybnntyjszdfndpdhtqzmbqlxlcmyffgtjjqwftmnpjwdnlbzcmmcngbdzlqlpnfhyymjylsd"
"chdcjwjcctljcldtljjcbddpndsszycndbjlggjzxsxnlycybjjxxcbylzcfzppgkcxqdzfztjjfjdjxzbnzyjqctyjwhdyczhym"
"djxttmpxsplzcdwslshxypzgtfmlcjtacbbmgdewycyzxdszjyhflystygwhkjyylsjcxgywjcbllcsnddbtzbsclyzczzssqdll"
"mjyyhfllqllxfdyhabxggnywyypllsdldllbjcyxjznlhljdxyyqytdlllbngpfdfbbqbzzmdpjhgclgmjjpgaehhbwcqxajhhhz"
"chxyphjaxhlphjpgpzjqcqzgjjzzgzdmqyybzzphyhybwhazyjhykfgdpfqsdlzmljxjpgalxzdaglmdgxmmzqwtxdxxpfdmmssy"
"mpfmdmmkxksyzyshdzkjsysmmzzzmdydyzzczxbmlstmdyemxckjmztyymzmzzmsshhdccjewxxkljsthwlsqlyjzllsjssdppmh"
"nlgjczyhmxxhgncjmdhxtkgrmxfwmckmwkdcksxqmmmszzydkmsclcmpcjmhrpxqpzdsslcxkyxtwlkjyahzjgzjwcjnxyhmmbml"
"gjxmhlmlgmxctkzmjlyscjsyszhsyjzjcdajzhbsdqjzgwtkqxfkdmsdjlfmnhkzqkjfeypzyszcdpynffmzqykttdzzefmzlbnp"
"plplpbpszalltnlkckqzkgenjlwalkxydpxnhsxqnwqnkxqclhyxxmlnccwlymqyckynnlcjnszkpyzkcqzqljbdmdjhlasqlbyd"
"wqlwdgbqcryddztjybkbwszdxdtnpjdtcnqnfxqqmgnseclstbhpwslctxxlpwydzklnqgzcqapllkqcylbqmqczqcnjslqzdjxl"
"ddhpzqdljjxzqdjyzhhzlkcjqdwjppypqakjyrmpzbnmcxkllzllfqpylllmbsglzysslrsysqtmxyxzqzbscnysyztffmzzsmzq"
"hzssccmlyxwtpzgxzjgzgsjzgkddhtqggzllbjdzlsbzhyxyzhzfywxytymsdnzzyjgtcmtnxqyxjscxhslnndlrytzlryylxqht"
"xsrtzcgyxbnqqzfhykmzjbzymkbpnlyzpblmcnqyzzzsjztjctzhhyzzjrdyzhnfxklfzslkgjtctssyllgzrzbbjzzklpkbczys"
"nnyxbjfbnjzzxcdwlzyjxzzdjjgggrsnjkmsmzjlsjywqsnyhqjsxpjztnlsnshrnynjtwchglbnrjlzxwjqxqkysjycztlqzybb"
"ybyzjqdwgyzcytjcjxckcwdkkzxsnkdnywwyyjqyytlytdjlxwkcjnklccpzcqqdzzqlcsfqchqqgssmjzzllbjjzysjhtsjdysj"
"qjpdszcdchjkjzzlpycgmzndjxbsjzzsyzyhgxcpbjydssxdzncglqmbtsfcbfdzdlznfgfjgfsmpnjqlnblgqcyyxbqgdjjqsrf"
"kztjdhczklbsdzcfytplljgjhtxzcsszzxstjygkgckgynqxjplzbbbgcgyjzgczqszlbjlsjfzgkqqjcgycjbzqtldxrjnbsxxp"
"zshszycfwdsjjhxmfczpfzhqhqmqnknlyhtycgfrzgnqxcgpdlbzcsczqlljblhbdcypscppdymzzxgyhckcpzjgslzlnscnsldl"
"xbmsdlddfjmkdqdhslzxlsznpqpgjdlybdskgqlbzlnlkyyhzttmcjnqtzzfszqktlljtyyllnllqyzqlbdzlslyyzxmdfszsnxl"
"xznczqnbbwskrfbcylctnblgjpmczzlstlxshtzcyzlzbnfmqnlxflcjlyljqcbclzjgnsstbrmhxzhjzclxfnbgxgtqncztmsfz"
"kjmssncljkbhszjntnlzdntlmmjxgzjyjczxyhyhwrwwqnztnfjscpyshzjfyrdjsfscjzbjfzqzchzlxfxsbzqlzsgyftzdcszx"
"zjbjpszkjrhxjzcgbjkhcggtxkjqglxbxfgtrtylxqxhdtsjxhjzjjcmzlcqsbtxwqgxtxxhxftsdkfjhzyjfjxnzldlllcqsqqz"
"qwqxswqtwgwbzcgcllqzbclmqjtzgzyzxljfrmyzflxnsnxxjkxrmjdzdmmyxbsqbhgzmwfwygmjlzbyytgzyccdjyzxsngnyjyz"
"nbgpzjcqsyxsxrtfyzgrhztxszzthcbfclsyxzlzqmzlmplmxzjssfsbysmzqhxxnxrxhqzzzsslyflczjrcrxhhzxqndshxsjjh"
"qcjjbcynsysxjbqjpxzqplmlxzkyxlxcnlcycxxzzlxdlllmjyhzxhyjwkjrwyhcpsgnrzlfzwfzznsxgxflzsxzzzbfcsyjdbrj"
"krdhhjxjljjtgxjxxstjtjxlyxqfcsgswmsbctlqzzwlzzkxjmltmjyhsddbxgzhdlbmyjfrzfcgclyjbpmlysmsxlszjqqhjzfx"
"gfqfqbphngyyqxgztnqwyltlgwgwwhnlfmfgzjmgmgbgtjflyzzgzyzaflsspmlbflcwbjztljjmzlpjjlymqtmyyyfbgygqzgly"
"zdxqyxrqqqhsxyyqxygjtyxfsfsllgnqcygycwfhcccfxpylypllzqxxxxxqqhhsshjzcftsczjxspzwhhhhhapylqnlpqafyhxd"
"ylnkmzqgggddesrenzltzgchyppcsqjjhclljtolnjpzljlhymhezdydsqycddhgznndzclzywllznteydgnlhslpjjbdgwxpcnn"
"tycklkclwkllcasstknzdnnjttlyyzssysszzryljqkcgdhhyrxrzydgrgcwcgzqffbppjfzynakrgywyjpqxxfkjtszzxswzddf"
"bbqtbgtzkznpzfpzxzpjszbmqhkyyxyldkljnypkyghgdzjxxeaxpnznctzcmxcxmmjxnkszqnmnlwbwwqjjyhclstmcsxnjcxxt"
"pcnfdtnnpglllzcjlspblpgjcdtnjjlyarscffjfqwdpgzdwmrzzcgodaxnssnyzrestyjwjyjdbcfxnmwttbqlwstszgybljpxg"
"lbnclgpcbjftmxzljylzxcltpnclcgxtfzjshcrxsfysgdkntlbyjcyjllstgqcbxnhzxbxklylhzlqzlnzcqwgzlgzjncjgcmnz"
"zgjdzxtzjxycyycxxjyyxjjxsssjstsstdppghtcsxwzdcsynptfbchfbblzjclzzdbxgcjlhpxnfzflsyltnwbmnjhszbmdnbcy"
"sccldnycndqlyjjhmqllcsgljjsyfpyyccyltjantjjpwycmmgqyysxdxqmzhszxbftwwzqswqrfkjlzjqqyfbrxjhhfwjgzyqac"
"myfrhcyybynwlpexcczsyyrlttdmqlrkmpbgmyyjprkznbbsqyxbhyzdjdnghpmfsgbwfzmfqmmbzmzdcgjlnnnxyqgmlrygqccy"
"xzlwdkcjcggmcjjfyzzjhycfrrcmtznzxhkqgdjxccjeascrjthpljlrzdjrbcqhjdnrhylyqjsymhzydwcdfryhbbydtssccwbx"
"glpzmlzjdqsscfjmmxjcxjytycghycjwynsxlfemwjnmkllswtxhyyyncmmcyjdqdjzglljwjnkhpzggflccsczmcbltbhbqjxqd"
"jpdjztghglfjawbzyzjltstdhjhctcbchflqmpwdshyytqwcnntjtlnnmnndyyyxsqkxwyyflxxnzwcxypmaelyhgjwzzjbrxxaq"
"jfllpfhhhytzzxsgqjmhspgdzqwbwpjhzjdyjcqwxkthxsqlzyymysdzgnqckknjlwpnsyscsyzlnmhqsyljxbcxtlhzqzpcycyk"
"pppnsxfyzjjrcemhszmnxlxglrwgcstlrsxbygbzgnxcnlnjlclynymdxwtzpalcxpqjcjwtcyyjlblxbzlqmyljbghdslssdmxm"
"bdczsxyhamlczcpjmcnhjyjnsykchskqmczqdllkablwjqsfmocdxjrrlyqchjmybyqlrhetfjzfrfksryxfjdwtsxxywsqjysly"
"xwjhsdlxyyxhbhawhwjcxlmyljcsqlkydttxbzslfdxgxsjkhsxxybssxdpwncmrptqzczenygcxqfjxkjbdmljzmqqxnoxslyxx"
"lylljdzptymhbfsttqqwlhsgynlzzalzxclhtwrrqhlstmypyxjjxmnsjnnbryxyjllyqyltwylqyfmlkljdnlltfzwkzhljmlhl"
"jnljnnlqxylmbhhlnlzxqchxcfxxlhyhjjgbyzzkbxscqdjqdsndzsygzhhmgsxcsymxfepcqwwrbpyyjqryqcyjhqqzyhmwffhg"
"zfrjfcdbxntqyzpcyhhjlfrzgpbxzdbbgrqstlgdgylcqmgchhmfywlzyxkjlypjhsywmqqggzmnzjnsqxlqsyjtcbehsxfszfxz"
"wfllbcyyjdytdthwzsfjmqqyjlmqsxlldttkghybfpwdyysqqrnqwlgwdebzwcyygcnlkjxtmxmyjsxhybrwfymwfrxyymxysctz"
"ztfykmldhqdlgyjnlcryjtlpsxxxywlsbrrjwxhqybhtydnhhxmmywytycnnmnssccdalwztcpqpyjllqzyjswjwzzmmglmxclmx"
"nzmxmzsqtzppjqblpgxjzhfljjhycjsrxwcxsncdlxsyjdcqzxslqyclzxlzzxmxqrjmhrhzjbhmfljlmlclqnldxzlllfyprgjy"
"nxcqqdcmqjzzxhnpnxzmemmsxykynlxsxtljxyhwdcwdzhqyybgybcyscfgfsjnzdrzzxqxrzrqjjymcanhrjtldbpyzbstjhxxz"
"ypbdwfgzzrpymnnkxcqbyxnbnfyckrjjcmjegrzgyclnnzdnkknsjkcljspgyyclqqjybzssqlllkjftbgtylcccdblsppfylgyd"
"tzjqjzgkntsfcxbdkdxxhybbfytyhbclnnytgdhryrnjsbtcsnyjqhklllzslydxxwbcjqsbxnpjzjzjdzfbxxbrmladhcsnclbj"
"dstblprznswsbxbcllxxlzdnzsjpynyxxyftnnfbhjjjgbygjpmmmmsszljmtlyzjxswxtyledqpjmpgqzjgdjlqjwjqllsdgjgy"
"gmscljjxdtygjqjjjcjzcjgdzdshqgzjggcjhqxsnjlzzbxhsgzxcxyljxyxyydfqqjhjfxdhctxjyrxysqtjxyefyyssyxjxncy"
"zxfxcsxszxyyschshxzzzgzzzgfjdldylnpzgsjaztyqzpbxcbdztzczyxxyhhscjshcggqhjhgxhsctmzmehyxgebtclzkkwytj"
"zrslekestdbcyhqqsayxcjxwwgsphjszsdncsjkqcxswxfctynydpccczjqtcwjqjzzzqzljzhlsbhpydxpsxshhezdxfptjqyzc"
"xhyaxncfzyyhxgnqmywntzsjbnhhgymxmxqcnssbcqsjyxxtyyhybcqlmmszmjzzllcogxzaajzyhjmchhcxzsxsdznleyjjzjbh"
"zwjzsqtzpsxzzdsqjjjlnyazphhyysrnqzthzhnyjyjhdzxzlswclybzyecwcycrylchzhzydzydyjdfrjjhtrsqtxyxjrjhojyn"
"xelxsfsfjzghpzsxzszdzcqzbyyklsgsjhczshdgqgxyzgxchxzjwyqwgyhksseqzzndzfkwyssdclzstsymcdhjxxyweyxczayd"
"mpxmdsxybsqmjmzjmtjqlpjyqzcgqhyjhhhqxhlhdldjqcfdwbsxfzzyyschtytyjbhecxhjkgqfxbhyzjfxhwhbdzfyzbchpnpg"
"dydmsxhkhhmamlnbyjtmpxejmcthqbzyfcgtyhwphftgzzezsbzegpbmdskftycmhbllhgpzjxzjgzjyxzsbbqsczzlzscstpgxm"
"jsfdcczjzdjxsybzlfcjsazfgszlwbczzzbyztzynswyjgxzbdsynxlgzbzfygczxbzhzftpbgzgejbstgkdmfhyzzjhzllzzgjq"
"zlsfdjsscbzgpdlfzfzszyzyzsygcxsnxxchczxtzzljfzgqsqqxcjqccccdjcdszzyqjccgxztdlgscxzsyjjqtcclqdqztqchq"
"qyzynzzzpbkhdjfcjfztypqyqttynlmbdktjcpqzjdzfpjsbnjlgyjdxjdcqkzgqkxclbzjtcjdqbxdjjjstcxnxbxqmslyjcxnt"
"jqwwcjjnjjlllhjcwqtbzqqczczpzzdzyddcyzdzccjgtjfzdprntctjdcxtqzdtjnplzbcllctdsxkjzqdmzlbznbtjdcxfczdb"
"czjjltqqpldckztbbzjcqdcjwynllzlzccdwllxwzlxrxntqjczxkjlsgdnqtddglnlajjtnnynkqlldzntdnycygjwyxdxfrsqs"
"tcdenqmrrqzhhqhdldazfkapbggpzrebzzykyqspeqjjglkqzzzjlysyhyzwfqznlzzlzhwcgkypqgnpgblplrrjyxcccgyhsfzf"
"wbzywtgzxyljczwhncjzplfflgskhyjdeyxhlpllllcygxdrzelrhgklzzyhzlyqszzjzqljzflnbhgwlczcfjwspyxzlzlxgccp"
"zbllcxbbbbnbbcbbcrnnzccnrbbnnldcgqyyqxygmqzwnzytyjhyfwtehznjywlccntzyjjcdedpwdztstnjhtymbjnyjzlxtsst"
"phndjxxbyxqtzqddtjtdyztgwscszqflshlnzbcjbhdlyzjyckwtydylbnydsdsycctyszyyebgexhqddwnygyclxtdcystqnygz"
"ascsszzdzlcclzrqxyywljsbymxshzdembbllyyllytdqyshymrqnkfkbfxnnsbychxbwjyhtqbpbsbwdzylkgzskyghqzjxhxjx"
"gnljkzlyycdxlfwfghljgjybxblybxqpqgntzplncybxdjyqydymrbeyjyyhkxxstmxrczzjwxyhybmcflyzhqyzfwxdbxbcwzms"
"lpdmyckfmzklzcyqycclhxfzlydqzpzygyjyzmdxtzfnnyttqtzhgsfcdmlccytzxjcytjmkslpzhysnwllytpzctzccktxdhxxt"
"qcyfksmqccyyazhtjplylzlyjbjxtfnyljyynrxcylmmnxjsmybcsysslzylljjgyldzdlqhfzzblfndsqkczfyhhgqmjdsxyctt"
"xnqnjpyybfcjtyyfbnxejdgyqbjrcnfyyqpghyjsyzngrhtknlnndzntsmgklbygbpyszbydjzsstjztsxzbhbscsbzczptqfzlq"
"flypybbjgszmnxdjmtsyskkbjtxhjcegbsmjyjzcstmljyxrczqscxxqpyzhmkyxxxjcljyrmyygadyskqlnadhrskqxzxztcggz"
"dlmlwxybwsyctbhjhcfcwzsxwwtgzlxqshnyczjxemplsrcgltnzntlzjcyjgdtclglbllqpjmzpapxyzlaktkdwczzbncctdqqz"
"qyjgmcdxltgcszlmlhbglkznnwzndxnhlnmkydlgxdtwcfrjerctzhydxykxhwfzcqshknmqqhzhhymjdjskhxzjzbzzxympajnm"
"ctbxlsxlzynwrtsqgscbptbsgzwyhtlkssswhzzlyytnxjgmjrnsnnnnlskztxgxlsammlbwldqhylakqcqctmycfjbslxclzjcl"
"xxknbnnzlhjphqplsxsckslnhpsfqcytxjjzljldtzjjzdlydjntptnndskjfsljhylzqqzlbthydgdjfdbyadxdzhzjnthqbykn"
"xjjqczmlljzkspldsclbblnnlelxjlbjycxjxgcnlcqplzlznjtsljgyzdzpltqcssfdmnycxgbtjdcznbgbqyqjwgkfhtnbyqzq"
"gbkpbbyzmtjdytblsqmbsxtbnpdxklemyycjynzdtldykzzxtdxhqshygmzsjycctayrzlpwltlkxslzcggexclfxlkjrtlqjaqz"
"ncmbqdkkcxglczjzxjhptdjjmzqykqsecqzdshhadmlzfmmzbgntjnnlhbyjbrbtmlbyjdzxlcjlpldlpcqdhlhzlycblcxccjad"
"qlmzmmsshmybhbnkkbhrsxxjmxmdznnpklbbrhgghfchgmnklltsyyycqlcskymyehywxnxqywbawykqldnntndkhqcgdqktgpkx"
"hcpdhtwnmssyhbwcrwxhjmkmzngwtmlkfghkjyldyycxwhyyclqhkqhtdqkhffldxqwytyydesbpkyrzpjfyyzjceqdzzdlattpb"
"fjllcxdlmjsdxegwgsjqxcfbssszpdyzcxznyxppzydlyjccpltxlnxyzyrscyyytylwwndsahjsygyhgywwaxtjzdaxysrltdps"
"syxfnejdxyzhlxlllzhzsjnyqyqyxyjghzgjcyjchzlycdshhsgczyjscllnxzjjyyxnfsmwfpyllyllabmddhwzxjmcxztzpmlq"
"chsfwzynctlndywlslxhymmylmbwwkyxyaddxylldjpybpwnxjmmmllhafdllaflbnhhbqqjqzjcqjjdjtffkmmmpythygdrjrdd"
"wrqjxnbysrmzdbyytbjhpymyjtjxaahggdqtmystqxkbtzbkjlxrbyqqhxmjjbdjntgtbxpgbktlgqxjjjcdhxqdwjlwrfmjgwqh"
"cnrxswgbtgygbwhswdwrfhwytjjxxxjyzyslphyypyyxhydqpxshxyxgskqhywbdddpplcjlhqeewjgsyykdpplfjthkjltcyjhh"
"jttpltzzcdlyhqkcjqysteeyhkyzyxxyysddjkllpymqyhqgxqhzrhbxpllnqydqhxsxxwgdqbshyllpjjjthyjkyphthyyktyez"
"yenmdshlzrpqfbnfxzbsftlgxsjbswyysksflxlpplbbblnsfbfyzbsjssylpbbffffsscjdstjsxtryjcyffsyzyzbjtlctsbsd"
"hrtjjbytcxyyeylycbnebjdsysyhgsjzbxbytfzwgenhhhthjhhxfwgcstbgxklstyymtmbyxjskzscdyjrcythxzfhmymcxlzns"
"djtxtxrycfyjsbsdyerxhljxbbdeynjghxgckgscymblxjmsznskgxfbnbbthfjyafxwxfbxmyfhdttcxzzpxrsywzdlybbktyqw"
"qjbzypzjznjpzjlztfysbttslmptzrtdxqsjehbnylndxljsqmlhtxtjecxalzzspktlzkqqyfsyjywpcpqfhjhytqxzkrsgtksq"
"czlptxcdyyzsslzslxlzmacpcqbzyxhbsxlzdltztjtylzjyytbzypltxjsjxhlbmytxcqrblzssfjzztnjytxmyjhlhpblcyxqj"
"qqkzzscpzkswalqsplczzjsxgwwwygyatjbbctdkhqhkgtgpbkqyslbxbbckbmllndzstbklggqkqlzbkktfxrmdkbftpzfrtppm"
"ferqnxgjpzsstlbztpszqzsjdhljqlzbpmsmmsxlqqnhknblrddnhxdkddjcyyljfqgzlgsygmjqjkhbpmxyxlytqwlwjcpbmjxc"
"yzydrjbhtdjyeqshtmgsfyplwhlzffnynnhxqhpltbqpfbjwjdbygpnxtbfzjgnnntjshxeawtzylltyqbwjpgxghnnkndjtmszs"
"qynzggnwqtfhclssgmnnnnynzqqxncjdqgzdlfnykljcjllzlmzznnnnsshthxjlzjbbhqjwwycrdhlyqqjbeyfsjhthnrnwjhwp"
"slmssgzttygrqqwrnlalhmjtqjsmxqbjjzjqzyzkxbjqxbjxshzssfglxmxnxfghkzszggslcnnarjxhnlllmzxelglxydjytlfb"
"kbpnlyzfbbhptgjkwetzhkjjxzxxglljlstgshjjyqlqzfkcgnndjsszfdbctwwseqfhqjbsaqtgypjlbxbmmywxgslzhglsgnyf"
"ljbyfdjfngsfmbyzhqffwjsyfyjjphzbyyzffwotjnlmftwlbzgyzqxcdjygzyyryzynyzwegazyhjjlzrthlrmgrjxzclnnnljj"
"yhtbwjybxxbxjjtjteekhwslnnlbsfazpqqbdlqjjtyyqlyzkdksqjnejzldqcgjqnnjsncmrfqthtejmfctyhypymhydmjncfgy"
"yxwshctxrljgjzhzcyyyjltkttntmjlzclzzayyoczlrlbszywjytsjyhbyshfjlykjxxtmzyyltxxypslqyjzyzyypnhmymdyyl"
"blhlsyygqllnjjymsoycbzgdlyxylcqyxtszegxhzglhwbljheyxtwqmakbpqcgyshhegqcmwyywljyjhyyzlljjylhzyhmgsljl"
"jxcjjyclycjbcpzjzjmmwlcjlnqljjjlxyjmlszljqlycmmgcfmmfpqqmfxlqmcffqmmmmhnznfhhjgtthxkhslnchhyqzxtmmqd"
"cydyxyqmyqylddcyaytazdcymdydlzfffmmycqcwzzmabtbyctdmndzggdftypcgqyttssffwbdttqssystwnjhjytsxxylbyyhh"
"whxgzxwznnqzjzjjqjccchykxbzszcnjtllcqxynjnckycynccqnxyewyczdcjycchyjlbtzyycqwlpgpyllgktltlgkgqbgychj"
"xy";
#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 在entry_id对应的列表中为某个entry_id赋值，并为spare_id赋值
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: spare_id是列表中最小的未被使用的entry_id
/*****************************************************************************/
LOCAL void SetEntryIdFlag
(
 BOOLEAN                is_add,         //IN: TRUE，表示置1；FALSE，表示置0
 uint16              entry_id,      //IN: 从1开始
 MMIPB_ENTRY_ID_T   *entry_id_ptr   //IN/OUT: entry_id列?                       
 );

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int ContactListCmp(uint32 base_index, void *compare_data, void *list);

/*****************************************************************************/
//  Description : get contact email
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: group
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsContactHasMail(
                      MMIPB_CONTACT_LIST_NODE_T *node 
                     );


/*****************************************************************************/
//  Description : get num_type
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: num_type
//  Note: 
/*****************************************************************************/
LOCAL void GetContactNumType(
                      MMIPB_CONTACT_LIST_NODE_T *node, 
                      uint8 num_type[],//OUT
                      uint8 type_count
                     );

/*****************************************************************************/
//  Description : change storage id to short storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint8 StorageIDToShortID(
                         uint16 storage_id
                         );

/*****************************************************************************/
//  Description : change short storage id to storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint16 ShortIDToStorageID(
                         uint8 short_storage_id
                         );

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : Find dail number contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: msisdn, fdn, sdn...
/*****************************************************************************/
LOCAL MMIPB_ERROR_E FindDailNumberContact(
                                       uint16 start_index,   
                                       uint16 storage_id, //要查找的联系人类型
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       );
#endif
/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListCmp(uint32 base_index, void *compare_data, void* list);

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListNCmp(uint32 base_index, void *compare_data, void *list);

/*****************************************************************************/
//  Description :根据字符串在快速查找表中查找对应的index
//str_input:需要查找的字符串
//is_back: TRUE:查找最后一个匹配的位置;FALSE:查找第一个匹配的位置
// 返回匹配的位置
/*****************************************************************************/
LOCAL uint16 GetQSortListIndex
(
    MMI_STRING_T *str_input,//IN
    BOOLEAN      is_char,//IN
    BOOLEAN      is_back
); 

#ifdef MMI_PB_MULTILANG_SORT_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIPB_MultiLang_FirstCharBinarySearch(MMIPB_MULTILANG_FIRST_CHAR_T* first_char_array, int first_char_array_len, uint16 value);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_MultiLang_GetUnicodeSortNum(uint16 text_unicode);
#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   
//快速查找表
LOCAL MMIPB_QSORT_LIST_T s_pb_qsort_list = {0};
//记录索引表
LOCAL MMIPB_CONTACT_LIST_T s_pb_contact_list = {0};

//手机联系人缓存表
LOCAL MMIPB_PHONE_CONTACT_LIST_T s_pb_phone_contact_list = {0};
#ifdef SNS_SUPPORT
//sns联系人缓存表
LOCAL MMIPB_SNS_CONTACT_LIST_T s_pb_sns_contact_list = {0};
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
//常用联系人索引表
LOCAL MMIPB_MOSTUSED_CONTACT_LIST_T s_pb_mostused_contact_list = {0};
LOCAL MMIPB_ENTRY_ID_T s_pb_mostused_contact_id_table = {0};
#endif
//contact id的使用情况表
LOCAL MMIPB_ENTRY_ID_T s_pb_phone_contact_id_table = {0};

LOCAL MPOOL_HANDLE     s_pb_mempool_handle = 0;

#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
LOCAL MPOOL_HANDLE     s_pb_only_sim_mempool_handle = 0;
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/

//semphore
LOCAL SCI_SEMAPHORE_PTR              s_contact_list_semphore_p = PNULL;        //联系人列表的访问信号量
#ifdef MMIPB_MOST_USED_SUPPORT
LOCAL SCI_SEMAPHORE_PTR              s_mostusedcontext_list_semphore_p = PNULL;//常用联系人列表的访问信号量
#endif
LOCAL SCI_SEMAPHORE_PTR              s_qsort_list_semphore_p = PNULL;          //快速查找的访问信号量


#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
LOCAL BOOLEAN s_pb_only_sim_mem_alloc = FALSE;
//SIM 快速查找表
LOCAL MMIPB_QSORT_LIST_T s_pb_only_sim_qsort_list = {0};
//SIM 记录索引表
LOCAL MMIPB_CONTACT_LIST_T s_pb_only_sim_contact_list = {0};
LOCAL SCI_SEMAPHORE_PTR              s_contact_only_sim_list_semphore_p = PNULL; 
LOCAL SCI_SEMAPHORE_PTR              s_qsort_only_sim_list_semphore_p = PNULL; 
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/

#ifdef MMIPB_SYNC_WITH_O365
//LOCAL MMIPB_SYNC_PHONE_UPDATE_LIST_T *cur_pb_update_node = PNULL;
LOCAL MMIPB_SYNC_TYPE_E cur_sync_type = MMIPB_SYNC_TYPE_SUSPEND;
LOCAL MMIPB_SYNC_TIME_INFO_T sync_time_info = {0};
//LOCAL uint8    sync_heart_beat_timer_id = 0;
LOCAL BOOLEAN isUpToDate = FALSE;

#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 在entry_id对应的列表中为某个entry_id赋值，并为spare_id赋值
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: spare_id是列表中最小的未被使用的entry_id
/*****************************************************************************/
LOCAL void SetEntryIdFlag
(
 BOOLEAN                is_add,         //IN: TRUE，表示置1；FALSE，表示置0
 uint16              entry_id,      //IN: 从1开始
 MMIPB_ENTRY_ID_T   *entry_id_ptr   //IN/OUT: entry_id列?                       
 )
{
    uint16   index_value = 0;
    uint16    i = 0;
    uint16    j = 0;
    uint16    k = 0;
    
    uint16   first_array_pos = 0;
    uint16   last_array_pos  = 0;
    
    uint16   last_byte_pos  = 0;
    
    BOOLEAN is_spare_id = FALSE;
    
    MMIPB_ENTRY_ID_FLAG_T   flag_t = {0};
    MMIPB_ENTRY_ID_FLAG_T   max_flag_t = {0};
    //MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_1;

    if(0 == entry_id || PNULL == entry_id_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]SetEntryIdFlag entry_id 0x%x pb_sort_ptr %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_185_112_2_18_2_43_18_174,(uint8*)"dd", entry_id, entry_id_ptr);
        return;
    }     
    // 找到待修改的entry_id在第几个byte中的第几个bit
    flag_t.array_pos    = ((entry_id - 1) >> 3);
    flag_t.byte_pos     = (uint8)((entry_id - 1) & 0x07);
    
    // 找到entry_id的最大值在第几个byte中的第几个bit
    max_flag_t.array_pos    = ((entry_id_ptr->max_record_num - 1) >> 3);
    max_flag_t.byte_pos     = (uint8)((entry_id_ptr->max_record_num - 1) & 0x07);
    
    if (is_add)
    {
        // 在对应entry_id的位置置1
        entry_id_ptr->valid_flag[flag_t.array_pos] |= (MMIPB_MASK_ENTRY_ID_FLAG_1 << flag_t.byte_pos);
        
        // 正好使用的是spare_id
        if (entry_id == entry_id_ptr->spare_id)
        {
            // 为spare_id赋一个无效的值
            entry_id_ptr->spare_id = MMIPB_INVALID_MAX_ENTRY_ID;
            
            first_array_pos = flag_t.array_pos;
            last_array_pos  = max_flag_t.array_pos;
            
            // 找到下一个spare_id的位置
            // 分段查找
            // 先从目前空闲记录号所在的BYTE开始查找
            // 如果没有找到，再从最前面开始找到目前空闲记录号所在的BYTE
            while ((!is_spare_id) && (k < 2))
            {
                // 从当前记录号对应BYTE的最低位开始查找
                // 如果没有记录，该BYTE会查找两次
                for (i=first_array_pos; i<=last_array_pos; i++)
                {
                    // 如果不是最后一个BYTE, 表示有空闲的记录号
                    // 如果是最后一个BYTE, 则不一定
                    if (0xff != entry_id_ptr->valid_flag[i])
                    {
                        last_byte_pos   = 7;
                        
                        if (i == max_flag_t.array_pos)
                        {
                            last_byte_pos = max_flag_t.byte_pos;
                        }
                        
                        for (j=0; j<=last_byte_pos; j++)
                        {
                            if (!(entry_id_ptr->valid_flag[i] & (MMIPB_MASK_ENTRY_ID_FLAG_1 << j)))
                            {
                                // 如果该位没有被置为1
                                index_value = i;
                                entry_id_ptr->spare_id = (index_value << 3) + j + 1;
                                is_spare_id = TRUE;
                                return;
                            }
                        }
                    } // end of if
                } // end of for
                
                k++;
                
                if (1 == k)
                {
                    // 查找的范围是前一部分
                    first_array_pos = 0;
                    last_array_pos  = flag_t.array_pos;
                }
                
            } // end of while
        }
    }
    else
    {
        // 在对应entry_id的位置置0
        entry_id_ptr->valid_flag[flag_t.array_pos] &= ~(uint8)(MMIPB_MASK_ENTRY_ID_FLAG_1 << flag_t.byte_pos);
        
        // 将spare_id指向更小的值
        if (entry_id < entry_id_ptr->spare_id)
        {
            entry_id_ptr->spare_id = entry_id;
        }
    }
}

/*****************************************************************************/
//  Description : get contact email
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: group
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsContactHasMail(
                      MMIPB_CONTACT_LIST_NODE_T *node
                     )
{
    BOOLEAN result = FALSE;
    if(PNULL == node) return result;
#ifdef MMIPB_MAIL_SUPPORT
    if(MMIPB_IsPhoneContact(ShortIDToStorageID(node->short_storage_id)))
    {
        if(node->index < s_pb_phone_contact_list.count_count)
        {
            if(s_pb_phone_contact_list.contact_table[node->index].mail_str.wstr_len > 0
              &&s_pb_phone_contact_list.contact_table[node->index].mail_str.wstr_len < MMIPB_EMPTY_STRING_FLAG   
              )
            {
                result = TRUE;
            }
        }


#ifndef MMIPB_MEMPOOL_SUPPORT
        else
        {
            MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        
            if(contact_ptr != PNULL)
            {
                MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
                MMIPB_CONTACT_T *contact_ptr = PNULL; 
                MMIPB_ACCESS_POINT_T access_point = {0};
        
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = node->contact_id;
                access_point.contact.storage_id =ShortIDToStorageID(node->short_storage_id);
                ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == ret)
                {
                    if(contact_ptr->mail.wstr_len>0)
                    {
                        result = TRUE;
                    }
                }
                SCI_FREE(contact_ptr);
            }
        }      
#endif
    }
#ifdef CMCC_VCARD_SUPPORT
        else if(MMIPB_IsUsim(ShortIDToStorageID(node->short_storage_id)))
        {
            if(node->num_mail_list != PNULL && node->num_mail_list->mail_str.wstr_len > 0)
            {
                result = TRUE;
            }
        }
#endif 
#endif 
    return result;
}

/*****************************************************************************/
//  Description : get num_type
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: num_type
//  Note: 
/*****************************************************************************/
LOCAL void GetContactNumType(
                      MMIPB_CONTACT_LIST_NODE_T *node,
                      uint8 num_type[],//OUT
                      uint8 type_count
                     )
{

    if(num_type == PNULL || type_count == 0 || node == PNULL)
    {
        return;
    }
    if(MMIPB_IsPhoneContact(ShortIDToStorageID(node->short_storage_id)))
    {
        if(node->index < s_pb_phone_contact_list.count_count)
        {
            SCI_MEMCPY(num_type, s_pb_phone_contact_list.contact_table[node->index].num_type, sizeof(uint8)*type_count);
        }


#ifndef MMIPB_MEMPOOL_SUPPORT
        else
        {
            MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
                MMIPB_ACCESS_POINT_T access_point = {0};
        
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = node->contact_id;
                access_point.contact.storage_id =ShortIDToStorageID(node->short_storage_id);
                ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == ret)
                {
                    SCI_MEMCPY(num_type, contact_ptr->num_type, sizeof(uint8)*type_count);
                }
                SCI_FREE(contact_ptr);
            }
        }
#endif
    }
	//get num_type if is sim record, for Bug 113812
    else if (MMIPB_IsSimRecord(ShortIDToStorageID(node->short_storage_id)))
    {
		MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
		if(contact_ptr != PNULL)
		{
			MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
			MMIPB_ACCESS_POINT_T access_point = {0};
			
			SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
			access_point.contact.contact_id = node->contact_id;
			access_point.contact.storage_id =ShortIDToStorageID(node->short_storage_id);
			ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
				access_point,
				sizeof(MMIPB_CONTACT_T),
				contact_ptr
				);
			if(MMIPB_ERROR_SUCCESS == ret)
			{
				SCI_MEMSET(num_type, 0X00, type_count*sizeof(uint8));
				SCI_MEMCPY(num_type, contact_ptr->num_type, sizeof(uint8));
			}
            SCI_TRACE_LOW("[MMIPB]GetContactNumType:%d is sim contact\n", contact_ptr->num_type);
			SCI_FREE(contact_ptr);
		}
	}
#ifdef CMCC_VCARD_SUPPORT
    if(MMIPB_IsUsim(ShortIDToStorageID(node->short_storage_id)))
    {
        MMIPB_BCD_NUMBER_LIST_T*	number_list = PNULL;
        uint8 						num_index = 1;

        SCI_TRACE_LOW("xiaohua GetContactNumType is usim contact\n");
        if(node->num_mail_list != PNULL)
        {
            number_list = node->num_mail_list->number_list;
            while(num_type[num_index]>0&&num_index < type_count)
            {
                num_index++;
            }   
        }
        SCI_TRACE_LOW("xiaohua GetContactNumType number_list:%d num_index %d\n",number_list,num_index);

        while(number_list != PNULL && num_index < type_count)
        {
            num_type[num_index] = number_list->num_type;
            number_list = number_list->next_ptr;
            num_index++;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : change storage id to short storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint8 StorageIDToShortID(
                         uint16 storage_id
                         )
{
    uint8 shor_id = (uint8)((storage_id &0x0F00) >> 4) + (storage_id &0xF);

    return shor_id;
   
}

/*****************************************************************************/
//  Description : change short storage id to storage id
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: storage id
//  Note: 
/*****************************************************************************/
LOCAL uint16 ShortIDToStorageID(
                         uint8 short_storage_id
                         )
{
    uint16 storage_id = ((short_storage_id & 0xF0) << 4) + (short_storage_id &0x0F);

    return storage_id;
   
}

/*****************************************************************************/
//  Description : 从联系人索引表中读取联系人姓名
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: MMIPB_ERROR_SUCCESS:读成功; 返回其他值 :操作失败
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactNameNumberByIndex(
                                                 MMIPB_NAME_NUMBER_MAIL_T *cmp_str,//[OUT]: 比较字符串
                                                 uint16        pos     //IN: 在存储器中的位置
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 storage_id = 0;
    uint16 cache_index = 0;
    MMI_STRING_T name_str = {0};
#ifdef MMI_PB_SURNAME_SUPPORT
    MMI_STRING_T surname_str = {0};
#endif /*MMI_PB_SURNAME_SUPPORT*/
#ifdef MMIPB_MAIL_SUPPORT
    MMI_STRING_T mail_str = {0};
#endif
    MMIPB_CONTACT_T *contact_ptr = PNULL; 
    MMIPB_ACCESS_POINT_T access_point = {0};
   
    if((PNULL == cmp_str))
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetContactNameNumberByIndex invalid param "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_382_112_2_18_2_43_19_175,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }  
    //SCI_TRACE_LOW("[MMIPB]MMIPB_GetContactNameNumberByIndex pos %d",pos);
    
    if(s_pb_contact_list.list != PNULL && pos < s_pb_contact_list.list_info.count)
    {
        storage_id = ShortIDToStorageID(s_pb_contact_list.list[pos].short_storage_id);

        cache_index = s_pb_contact_list.list[pos].index;
        //read name
        if(MMIPB_IsPhoneContact(storage_id) && (cache_index < s_pb_phone_contact_list.count_count))
        {
            name_str = s_pb_phone_contact_list.contact_table[cache_index].name_str;
#ifdef MMI_PB_SURNAME_SUPPORT
            surname_str = s_pb_phone_contact_list.contact_table[cache_index].surname_str;
#endif /*MMI_PB_SURNAME_SUPPORT*/
            SCI_MEMCPY(&cmp_str->number_t, &s_pb_phone_contact_list.contact_table[cache_index].number, sizeof(cmp_str->number_t));            
            SCI_MEMCPY(&cmp_str->num_type, &s_pb_phone_contact_list.contact_table[cache_index].num_type, sizeof(cmp_str->num_type));
#ifdef MMIPB_MAIL_SUPPORT
            mail_str = s_pb_phone_contact_list.contact_table[cache_index].mail_str;
#endif
            cmp_str->group = s_pb_contact_list.list[pos].group;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            cmp_str->preferred_sim =  s_pb_phone_contact_list.contact_table[cache_index].preferred_sim;
#endif
        }

#ifdef SNS_SUPPORT
        else if(MMIPB_IsSNSContact(storage_id) &&(cache_index < s_pb_sns_contact_list.current_count))
        {
            name_str = s_pb_sns_contact_list.contact_table[cache_index].name_str;
        }
#endif

        if((MMIPB_EMPTY_STRING_FLAG != name_str.wstr_len && name_str.wstr_ptr == PNULL)           
#ifdef MMIPB_MAIL_SUPPORT
            ||(MMIPB_EMPTY_STRING_FLAG != mail_str.wstr_len && mail_str.wstr_ptr == PNULL)
#endif
#ifdef MMI_PB_SURNAME_SUPPORT
            ||(MMIPB_EMPTY_STRING_FLAG != surname_str.wstr_len && surname_str.wstr_ptr == PNULL)
#endif  /*MMI_PB_SURNAME_SUPPORT*/
            )
        {
            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = s_pb_contact_list.list[pos].contact_id;
                access_point.contact.storage_id = storage_id;
                result = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == result)
                {
                    SCI_MEMCPY(&cmp_str->name, &contact_ptr->name, sizeof(cmp_str->name));
#ifdef MMI_PB_SURNAME_SUPPORT
                    SCI_MEMCPY(&cmp_str->surname, &contact_ptr->surname, sizeof(cmp_str->surname));
#endif /*MMI_PB_SURNAME_SUPPORT*/
                    SCI_MEMCPY(&cmp_str->number_t, contact_ptr->number, sizeof(cmp_str->number_t));
                    SCI_MEMCPY(&cmp_str->num_type, contact_ptr->num_type, sizeof(cmp_str->num_type));
#ifdef MMIPB_MAIL_SUPPORT
                    SCI_MEMCPY(&cmp_str->mail, &contact_ptr->mail, sizeof(cmp_str->mail));
#endif
                    cmp_str->group = contact_ptr->union_t.group;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                    cmp_str->preferred_sim = contact_ptr->preferred_sim;
#endif
                     result = MMIPB_ERROR_SUCCESS;
                }
                SCI_FREE(contact_ptr);
            }
           
        }
        else
        {
            if(name_str.wstr_len > 0 && name_str.wstr_ptr != PNULL)
            {
                cmp_str->name.wstr_len = MIN(name_str.wstr_len, MMIPB_NAME_MAX_LEN);
                SCI_MEMCPY(&cmp_str->name.wstr, name_str.wstr_ptr, sizeof(wchar)*cmp_str->name.wstr_len);               
            }

#ifdef MMI_PB_SURNAME_SUPPORT			
            if(surname_str.wstr_len > 0 && surname_str.wstr_ptr != PNULL)
            {
                cmp_str->surname.wstr_len = MIN(surname_str.wstr_len, MMIPB_NAME_MAX_LEN);
                SCI_MEMCPY(&cmp_str->surname.wstr, surname_str.wstr_ptr, sizeof(wchar)*cmp_str->surname.wstr_len);               
            }
#endif /*MMI_PB_SURNAME_SUPPORT*/

#ifdef MMIPB_MAIL_SUPPORT
            if(mail_str.wstr_len > 0 && mail_str.wstr_ptr != PNULL)
            {
                cmp_str->mail.wstr_len = MIN(mail_str.wstr_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMCPY(&cmp_str->mail.wstr, mail_str.wstr_ptr, sizeof(wchar)*cmp_str->mail.wstr_len);               
            }
#endif
            result = MMIPB_ERROR_SUCCESS;
        }
    }
    return (result);
}

/*****************************************************************************/
//  Description :调整记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E AdjustContactListInfo(
                                          MMIPB_CONTACT_BASE_INFO_T *base_info_ptr,//[IN]
                                          BOOLEAN is_increase,
                                          MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                                          )
{
    
    uint16 i = 0;
    uint32 group_val = 0;
    uint16 num_count = 0;
    uint16 mail_count = 0;

    if(data_ptr == PNULL || base_info_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //计算number个数
    for (i=0; i<MMIPB_MAX_NV_PHONE_NUM; i++)
    {
        if (base_info_ptr->num_type[i] != MMIPB_INVLID_NUMBER)
        {
            num_count++;
        }
    }
    
    // 计算邮箱的个数
    if (base_info_ptr->mail_exist)
    {
        mail_count++;
    }    
    //调整每个存储设备中的联系人数量
    if(MMIPB_GET_STORAGE(base_info_ptr->storage_id) < MMIPB_STORAGE_MAX)
    {
        if(is_increase)
        {
            data_ptr->storage_count[MMIPB_GET_STORAGE(base_info_ptr->storage_id)]++;
        }
        else if(data_ptr->storage_count[MMIPB_GET_STORAGE(base_info_ptr->storage_id)] > 0)
        {
            data_ptr->storage_count[MMIPB_GET_STORAGE(base_info_ptr->storage_id)]--;
        }
    }
#ifdef TCARD_PB_SUPPORT	
    //adjust tcard contact count
    if(MMIPB_IsTCardContact(base_info_ptr->storage_id, base_info_ptr->contact_id))
    {
        if(is_increase)
        {
            data_ptr->tcontact_count++;
        }
        else if(data_ptr->tcontact_count > 0)
        {
            data_ptr->tcontact_count--;
        }
    }
#endif  

    //调整总号码个数和mail个数
    if(is_increase)
    {
        data_ptr->mail_count += mail_count;
        data_ptr->number_count += num_count;
    }
    else
    {
        if(data_ptr->mail_count >= mail_count)
        {
            data_ptr->mail_count -= mail_count;
        }
        if(data_ptr->number_count >= num_count)
        {
            data_ptr->number_count -= num_count;
        }
    }
    for(i =0; i < MMIPB_MAX_GROUP_NUM; i++)
    {
        group_val = (1 << i );
        if(group_val > base_info_ptr->group)
        {
            break;
        }
        
        if((group_val & base_info_ptr->group))
        {
            //调整各个分组的联系人数量，号码个数和mail个数
            if(is_increase)
            {
                data_ptr->group_count[i]++;
                data_ptr->group_mail_count[i] += mail_count;
                data_ptr->group_number_count[i] += num_count;
            }
            else
            {
                if(data_ptr->group_count[i] > 0)
                {
                    data_ptr->group_count[i]--;
                }
                if(data_ptr->group_mail_count[i] >= mail_count)
                {
                    data_ptr->group_mail_count[i] -= mail_count;
                }
                if(data_ptr->group_number_count[i] >= num_count)
                {
                    data_ptr->group_number_count[i] -= num_count;
                }
            }
        }
    }
    return MMIPB_ERROR_SUCCESS;
}


/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListCmp(uint32 base_index, void *compare_data, void *list)
{
  MMIPB_QSORT_LIST_T  *qsort_list = (MMIPB_QSORT_LIST_T * )list;
  //MMIPB_NAME_T         name     = {0};
  MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
  MMI_STRING_T       *input_str = (MMI_STRING_T *)compare_data;
  MMIPB_NAME_NUMBER_MAIL_T *cmp_str_ptr = PNULL;
  int ret = 0;
  BOOLEAN  is_char = TRUE;
  
  letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
  if(PNULL == letters_ptr)
  {
      return -1;
  }
  if(PNULL == qsort_list || PNULL == qsort_list->list)
  {
      //SCI_TRACE_LOW:"CompareLettersInSortTable valid parameter !!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_568_112_2_18_2_43_19_176,(uint8*)"");

      SCI_FREE(letters_ptr);
      return -1;
  }
  cmp_str_ptr = (MMIPB_NAME_NUMBER_MAIL_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));
  if(cmp_str_ptr != PNULL)
  {
      SCI_MEMSET(cmp_str_ptr, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
      MMIPB_GetContactNameNumberByIndex(cmp_str_ptr, (qsort_list->list[base_index] &MMIPB_CONTACT_INDEX_MASK));  
      //get name_in_list_t 第range_index个character对应的PinYin sn
      SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
      if(qsort_list->list[base_index] >= MMIPB_KEY_HEAD_FLAG)
      {
          //key value
          is_char = FALSE;
      }
      else
      {
          is_char = TRUE;
      }
#ifdef MMI_PB_SURNAME_SUPPORT
      MMIPB_GetNameLettersWithSurname(&cmp_str_ptr->name, &cmp_str_ptr->surname, MMIPB_NAME_HEAD_CHAR, is_char, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
      MMIPB_GetNameLetters(&cmp_str_ptr->name, MMIPB_NAME_HEAD_CHAR, is_char, letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
      ret = MMIAPICOM_Wstrcmp(letters_ptr->head_letters, input_str->wstr_ptr);
      SCI_FREE(cmp_str_ptr);
  }
  
  //SCI_TRACE_LOW("QSortListCmp ret %d ",ret);
  SCI_FREE(letters_ptr);
  return ret;
}

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QSortListNCmp(uint32 base_index, void *compare_data, void *list)
{
  MMIPB_QSORT_LIST_T  *qsort_list = (MMIPB_QSORT_LIST_T * )list;
  //MMIPB_NAME_T         name     = {0};
  MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
  MMIPB_QSORT_CMP_DATA_T  *cmp_data_ptr = (MMIPB_QSORT_CMP_DATA_T *)compare_data;
  //wchar               *name_str = PNULL;
  MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
  int ret = 0;

  if(cmp_data_ptr == PNULL || cmp_data_ptr->str_ptr == PNULL)
  {
        return -1;
  }
  letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
  if(PNULL == letters_ptr)
  {
      return -1;
  }
  if(PNULL == qsort_list || PNULL == qsort_list->list)
  {
      //SCI_TRACE_LOW:"CompareLettersInSortTable valid parameter !!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_615_112_2_18_2_43_19_177,(uint8*)"");
      SCI_FREE(letters_ptr);
      return -1;
  }
  MMIPB_GetContactNameNumberByIndex(&cmp_str, (qsort_list->list[base_index] & MMIPB_CONTACT_INDEX_MASK));  
  //get name_in_list_t 第range_index个character对应的PinYin sn
  SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
  if(qsort_list->list[base_index] >= MMIPB_KEY_HEAD_FLAG)
  {
#ifdef MMI_PB_SURNAME_SUPPORT
      MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_HEAD_CHAR, FALSE, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
      MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_HEAD_CHAR, FALSE, letters_ptr);          
#endif /*MMI_PB_SURNAME_SUPPORT*/
  }
  else
  {       
#ifdef MMI_PB_SURNAME_SUPPORT
      MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_HEAD_CHAR, TRUE, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
      MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_HEAD_CHAR, TRUE, letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
  }
  ret = MMIAPICOM_Wstrncmp(letters_ptr->head_letters, cmp_data_ptr->str_ptr->wstr_ptr, cmp_data_ptr->str_ptr->wstr_len);
  
  //SCI_TRACE_LOW("QSortListCmp ret %d ",ret);
  SCI_FREE(letters_ptr);
  return ret;
}

/*****************************************************************************/
//  Description :根据字符串在快速查找表中查找对应的index
//str_input:需要查找的字符串
//is_back: TRUE:查找最后一个匹配的位置;FALSE:查找第一个匹配的位置
// 返回匹配的位置
/*****************************************************************************/
LOCAL uint16 GetQSortListIndex
  (
  MMI_STRING_T *str_input,//IN
  BOOLEAN      is_char,//IN
  BOOLEAN      is_back
  )
{
  MMI_BIN_SEARCH_INFO_T search_info = {0};
  uint32                  ret_pos = 0;
  BIN_COMPARE_FUNC        func = PNULL;
  MMIPB_QSORT_CMP_DATA_T cmp_data = {0};
  
  if((PNULL!= s_pb_qsort_list.list) 
      && (s_pb_qsort_list.count > 0))
  {
      //use binary search
      if(is_back)
      {
          //查找最后一个相等的记录
          search_info.search_type = MMICOM_SEARCH_LAST_EQUAL;
      }
      else
      {
          //查找第一个相等的记录
          search_info.search_type = MMICOM_SEARCH_FIRST_EQUAL;
      }

      cmp_data.is_char = is_char;
      cmp_data.str_ptr = str_input;
      search_info.start_pos = 0;
      search_info.end_pos = s_pb_qsort_list.count-1;
      search_info.compare_base_data = &cmp_data;
      //find matched string in sort list table
      func = QSortListNCmp;
      if(MMIAPICOM_BinSearch
          (&search_info, 
          (BIN_COMPARE_FUNC)func,
          &ret_pos,
          &s_pb_qsort_list
          ))
      {
          ret_pos = PB_NOT_EXIST;
      }
  }
  //SCI_TRACE_LOW("[MMIPB]QSortListSearch ret_pos %d", ret_pos);
  return (uint16)ret_pos;
} 

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_ReleasePhoneContactListCache()
{
	if(s_pb_phone_contact_list.contact_table != PNULL)
	{
		uint16 index = 0;
		for(index = 0; index < s_pb_phone_contact_list.count_count; index++)
		{
			//删除从手机缓存表中删除该节点
			//delete name str 
			if(s_pb_phone_contact_list.contact_table[index].name_str.wstr_ptr != PNULL)
			{
				SCI_MPFREE(s_pb_phone_contact_list.contact_table[index].name_str.wstr_ptr);
				s_pb_phone_contact_list.contact_table[index].name_str.wstr_ptr = PNULL;
				s_pb_phone_contact_list.contact_table[index].name_str.wstr_len = 0;
			}
#ifdef MMI_PB_SURNAME_SUPPORT
			//surname
			if(s_pb_phone_contact_list.contact_table[index].surname_str.wstr_ptr != PNULL)
			{
				SCI_MPFREE(s_pb_phone_contact_list.contact_table[index].surname_str.wstr_ptr);
				s_pb_phone_contact_list.contact_table[index].surname_str.wstr_ptr = PNULL;
				s_pb_phone_contact_list.contact_table[index].surname_str.wstr_len = 0;
			}
#endif /*MMI_PB_SURNAME_SUPPORT*/
			//delete mail str
			if(s_pb_phone_contact_list.contact_table[index].mail_str.wstr_ptr != PNULL)
			{
				SCI_MPFREE(s_pb_phone_contact_list.contact_table[index].mail_str.wstr_ptr);
				s_pb_phone_contact_list.contact_table[index].mail_str.wstr_ptr = PNULL;
				s_pb_phone_contact_list.contact_table[index].mail_str.wstr_len = 0;
			}
		}
	}	
}

/*****************************************************************************/
//  Description : 初始化数据表
//  
// 
/*****************************************************************************/
PUBLIC void MMIPB_InitDataList(void)
{

  //uint32 sort_content_size = 0;
  MMIPB_STORAGE_INFO_T storage_info_t = {0};
  //nv init
  storage_info_t = MMIPB_GetContactStorage(MMI_DUAL_SYS_MAX, PHONEBOOK_NV_STORAGE);
  //total count
  //contact list size 
  //malloc contact list memory and init it
  /*
  if(s_pb_phone_contact_list.contact_table != PNULL)
  {
      SCI_FREE(s_pb_phone_contact_list.contact_table);
  }
  */

  if(s_pb_phone_contact_list.contact_table == PNULL)
  {
        s_pb_phone_contact_list.max_count = storage_info_t.max_record_num;
#ifdef MMIPB_MEMPOOL_SUPPORT
       s_pb_phone_contact_list.contact_table = SCI_ALLOC_CONST(sizeof(MMIPB_PHONE_CONTACT_NODE_T)* s_pb_phone_contact_list.max_count);
#else
       s_pb_phone_contact_list.contact_table = SCI_ALLOCA(sizeof(MMIPB_PHONE_CONTACT_NODE_T)* s_pb_phone_contact_list.max_count);
#endif
  }

  if(s_pb_phone_contact_list.contact_table != PNULL)
  {
          SCI_MEMSET(s_pb_phone_contact_list.contact_table, 
                    0x00, 
                    sizeof(MMIPB_PHONE_CONTACT_NODE_T)* (s_pb_phone_contact_list.max_count));
  }
  if(PNULL == s_pb_phone_contact_id_table.valid_flag)
  {
      s_pb_phone_contact_id_table.valid_flag= (uint8 *)SCI_ALLOCA(((storage_info_t.max_record_num + 7) / 8 ));
      s_pb_phone_contact_id_table.max_record_num = storage_info_t.max_record_num;
  }
  SCI_MEMSET(s_pb_phone_contact_id_table.valid_flag,0, ((s_pb_phone_contact_id_table.max_record_num+ 7) / 8 ));
  s_pb_phone_contact_id_table.spare_id = 1;
#ifdef MMIPB_MOST_USED_SUPPORT
  //常用联系人列表 
  if(PNULL == s_pb_mostused_contact_list.contact_table)
  {
      s_pb_mostused_contact_list.contact_table = (MMIPB_MOSTUSED_CONTACT_NODE_T *)SCI_ALLOC_CONST(MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS *sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
  }
  if(PNULL != s_pb_mostused_contact_list.contact_table)
  {
        SCI_MEMSET(s_pb_mostused_contact_list.contact_table,0, MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS *sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
  }
  //常用联系人id使用情况表
  if(PNULL == s_pb_mostused_contact_id_table.valid_flag)
  {
      s_pb_mostused_contact_id_table.valid_flag= (uint8 *)SCI_ALLOCA(((MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS + 7) / 8 ));
  }
  if(PNULL != s_pb_mostused_contact_id_table.valid_flag)
  {
        SCI_MEMSET(s_pb_mostused_contact_id_table.valid_flag,0, ((MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS + 7) / 8 ));
  }
  s_pb_mostused_contact_id_table.max_record_num = MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;
  s_pb_mostused_contact_id_table.spare_id = 1;
  s_pb_mostused_contact_list.count = 0;
  #endif
  //初始化总索引表，并分配空间,快速查找表和sns缓存表 
  s_pb_contact_list.list_info.count = 0;
  SCI_MEMSET(&s_pb_contact_list.list_info, 0x00, sizeof(MMIPB_CONTACT_LIST_INFO_T));
  s_pb_qsort_list.count = 0;
  s_pb_phone_contact_list.count_count = 0;
#ifdef SNS_SUPPORT
  s_pb_sns_contact_list.current_count = 0;
#endif

  //MMIPB_InitNumberList((MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX + (storage_info_t.max_record_num* MMIPB_MAX_NV_PHONE_NUM)));
}
#ifndef MMIPB_MEMPOOL_SUPPORT
/*****************************************************************************/
//  Description :释放string cache
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_FreeCache(void)
{
    SCI_FREE(s_pb_phone_contact_list.contact_table);
    s_pb_phone_contact_list.contact_table = PNULL;
    s_pb_phone_contact_list.count_count = 0;
#ifdef SNS_SUPPORT
    SCI_FREE(s_pb_sns_contact_list.contact_table);
    s_pb_sns_contact_list.contact_table = PNULL;
    s_pb_sns_contact_list.current_count= 0;
#endif
}
#endif
/*****************************************************************************/
//  Description : 分配索引表空间和快速查找表空间
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_ALLOCContactList(void)
{

  uint32  item_count = 0;
#ifdef SNS_SUPPORT
  uint32  sns_count = MMIPB_SNS_MAX_RECORD_NUM;//MMIPB_GetSNSContactsCount(0);
#endif
  MMIPB_STORAGE_INFO_T storage_info_t = MMIPB_GetContactStorage(MMI_DUAL_SYS_MAX, PHONEBOOK_NV_STORAGE);
#if defined(SNS_SUPPORT) // wang.qinggang Coverity for 26952
  item_count = (MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX) + storage_info_t.max_record_num + sns_count;
#elif defined(MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT) || defined(MMIPB_SYNC_WITH_O365)
  item_count = storage_info_t.max_record_num;
#else
    item_count = (MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX)+storage_info_t.max_record_num;
#endif

  //快速查找表的初始化和内存分配
  s_pb_qsort_list.list = (uint16 *)SCI_ALLOC_CONST(item_count *2*sizeof(uint16));
  if(s_pb_qsort_list.list != PNULL)
  {
      SCI_MEMSET(s_pb_qsort_list.list, 0x00, item_count*2 *sizeof(uint16));
      s_pb_qsort_list.max_count = item_count*2;
  }      
  
  //重新分配索引表 

  s_pb_contact_list.list = (MMIPB_CONTACT_LIST_NODE_T *)SCI_ALLOC_CONST(item_count *sizeof(MMIPB_CONTACT_LIST_NODE_T));
  if(s_pb_contact_list.list != PNULL)
  {
      SCI_MEMSET(s_pb_contact_list.list, 0x00, item_count*sizeof(MMIPB_CONTACT_LIST_NODE_T));
      s_pb_contact_list.max_count = item_count;
  }
  SCI_MEMSET(&s_pb_contact_list.list_info, 0x00, sizeof(MMIPB_CONTACT_LIST_INFO_T));
#ifdef SNS_SUPPORT
  //重新分配sns缓存表

  s_pb_sns_contact_list.contact_table = (MMIPB_SNS_CONTACT_NODE_T *)SCI_ALLOC_CONST(sizeof(MMIPB_SNS_CONTACT_NODE_T)*sns_count);
  if(s_pb_sns_contact_list.contact_table != PNULL)
  {
      SCI_MEMSET(s_pb_sns_contact_list.contact_table, 
          0x00, sizeof(MMIPB_SNS_CONTACT_NODE_T)*sns_count);
      s_pb_sns_contact_list.max_count = sns_count;
  }
  else
  {
      s_pb_sns_contact_list.max_count = 0;
  }
  s_pb_sns_contact_list.current_count = 0;
#endif
}
/*****************************************************************************/
//  Description : 创建数据表的访问控制信号量
//  
// 
/*****************************************************************************/
PUBLIC void MMIPB_CreateDataListSemphore(void)
{
    s_contact_list_semphore_p = SCI_CreateSemaphore("s_contact_list_semphore_p", 1);
#ifdef MMIPB_MOST_USED_SUPPORT    
    s_mostusedcontext_list_semphore_p = SCI_CreateSemaphore("s_mostusedcontext_list_semphore_p", 1);
#endif    
    s_qsort_list_semphore_p = SCI_CreateSemaphore("s_qsort_list_semphore_p", 1);    
}
/*****************************************************************************/
//  Description : 获取NV contact id的分配情况。
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfNV(uint8 *valid_buf, 
                                           uint8 buf_len
                                           )
{
    if(valid_buf != PNULL && buf_len > 0)
    {
        SCI_MEMCPY(valid_buf, s_pb_phone_contact_id_table.valid_flag, buf_len*sizeof(uint8));
    }
}

/*****************************************************************************/
//  Description : 获取sim contact id的分配情况。
//  Global resource dependence :  
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIdUsedInfoOfSIM(
                                            MN_DUAL_SYS_E dual_sys,
                                            uint8 *valid_buf, 
                                            uint16 max_count
                                           )
{
    uint32 i = 0;
    uint16 storage_id = 0;
    MMIPB_ENTRY_ID_T   entry_id_flag = {0};

    if(valid_buf != PNULL && max_count > 0 && dual_sys < MMI_DUAL_SYS_MAX)
    {
        entry_id_flag.spare_id = 1;
        entry_id_flag.max_record_num = max_count;
        entry_id_flag.valid_flag = valid_buf;
        for(i =0; i < s_pb_contact_list.list_info.count; i++)
        {
            storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
            if(MMIPB_IsSIMContact(storage_id) && dual_sys == MMIPB_GetDualSysByStorageId(storage_id))
            {
               SetEntryIdFlag(TRUE, s_pb_contact_list.list[i].contact_id, &entry_id_flag);
            }
        }
    }
}
/*****************************************************************************/
//  Description : Find visiable contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindContact(
                                       uint16 start_index,  
                                       uint32 group_mask,//[IN]所有要查找的群组
                                       uint16 storage_id, //要查找的联系人类型                                        
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 i = 0;
    uint16 j = 0;
    
    //get a visibile contact from start_index
    if(valid_index_array != PNULL)
    {
        //初始化有效号码索引和email数组
        SCI_MEMSET(valid_index_array->visible_num_index, 0x0, sizeof(valid_index_array->visible_num_index));
        valid_index_array->is_mail_visible = FALSE;
    }
    if(MMIPB_IsSIMContact(storage_id) 
       || MMIPB_IsPhoneContact(storage_id)
#ifdef SNS_SUPPORT        
       || MMIPB_IsSNSContact(storage_id) 
#endif
    )
    {
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
        if(s_pb_contact_list.list != PNULL)
        {
            for(i = start_index; i < s_pb_contact_list.list_info.count; i++)
            {
                if(( ((1<< PB_GROUP_ALL) &group_mask) || group_mask & s_pb_contact_list.list[i].group)//全部分组或包含要检索的分组
                    && s_pb_contact_list.list[i].visible_flag > 0//可见
                    )
                {
                    //如果要检索的分组是全部分组或该记录分组包含要检索的分组，并且该记录可见
                    result = MMIPB_ERROR_SUCCESS;
                    find_data_ptr->contact_id = s_pb_contact_list.list[i].contact_id;
                    find_data_ptr->first_char = s_pb_contact_list.list[i].first_char;
                    find_data_ptr->photo_id_flag = s_pb_contact_list.list[i].photo_id_flag;
                    find_data_ptr->group = s_pb_contact_list.list[i].group;
                    find_data_ptr->index = i;
                    find_data_ptr->mail_exist = IsContactHasMail(&(s_pb_contact_list.list[i]));
                    find_data_ptr->storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                    GetContactNumType(&(s_pb_contact_list.list[i]), find_data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
                    if(find_index != PNULL)
                    {
                        //set find index
                        *find_index = i; 
                    }
                    //get visible number index
                    for(j =0; j < MMIPB_MAX_NV_PHONE_NUM; j++)
                    {
                        
                        if(s_pb_contact_list.list[i].visible_flag < (1 << j))
                        {
                            break;
                        }
                        if(valid_index_array != PNULL && find_data_ptr->num_type[j] == MMIPB_INVLID_NUMBER)
                        {
//                            if(valid_index_array != PNULL)
                            {
                                valid_index_array->visible_num_index[j] = 0;
                            }
                        }
                        else if(s_pb_contact_list.list[i].visible_flag & (1 << j))
                        {
                            //该号码可见; this number is visible
                            if(valid_index_array != PNULL)
                            {
                                valid_index_array->visible_num_index[j] = 1;
                            }
                        }
                        
                    }
                    //set  visible 
                    if(valid_index_array != PNULL)
                    {
                        //该mail可见；this mail is visible
                        if(IsContactHasMail(&(s_pb_contact_list.list[i])) && (s_pb_contact_list.list[i].visible_flag &MMIPB_FEILD_FLAG_MASK_MAIL))
                        {
                            valid_index_array->is_mail_visible = TRUE;
                        }
                    }
                    break;
                }
            }
        }
        //SCI_RestoreIRQ();
        SCI_PutSemaphore(s_contact_list_semphore_p);
    }
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)    
    else
    {
        //other contact list
        result = FindDailNumberContact(start_index, storage_id, find_index, valid_index_array, find_data_ptr);
    }
#endif    
    //SCI_TRACE_LOW("MMIPB_FindContact start_index %d group_mask %d  result %d",start_index, group_mask,  result);
    return result;
}

#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : Find dail number contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: msisdn, fdn, sdn...
/*****************************************************************************/
LOCAL MMIPB_ERROR_E FindDailNumberContact(
                                       uint16 start_index,   
                                       uint16 storage_id, //要查找的联系人类型
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_INVALID_PARAM;    
    uint16 i =0;
    uint16 j =0;
    MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(storage_id);
    MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    MMIPB_ACCESS_POINT_T access_point = {0};

    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] FindDailNumberContact PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_981_112_2_18_2_43_20_178,(uint8*)"");
        return MMIPB_ERROR_ERROR;
    }
    for(i = start_index; i < storage_info.max_record_num; i++)
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
        access_point.contact.contact_id = i + 1;
        access_point.contact.storage_id = storage_id;
        if(MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_CONTACT,
            access_point,
            sizeof(MMIPB_CONTACT_T),
            contact_ptr
            ))
        {
            if(MMIPB_DailNumberContactCanbeEmpty(storage_id)
              ||MMIPB_IsContactValid(contact_ptr)  
              )
            {
                //find contact
                if(find_index != PNULL)
                {
                    //set index
                   *find_index = i;                   
                }
                if(find_data_ptr != PNULL)
                {
                    //set find contact base information
                    find_data_ptr->contact_id = access_point.contact.contact_id;
                    find_data_ptr->storage_id = storage_id;
                    find_data_ptr->index = i;
                    find_data_ptr->group = (1<<(PB_GROUP_SIM_1 + MMIPB_GetDualSysByStorageId(storage_id)));
                    find_data_ptr->visible_flag = 1;
#ifdef MMIPB_MAIL_SUPPORT
                    find_data_ptr->mail_exist = (contact_ptr->mail.wstr_len > 0 ? TRUE : FALSE);
#else
                    find_data_ptr->mail_exist = FALSE;
#endif
                    SCI_MEMCPY(find_data_ptr->num_type, contact_ptr->num_type, sizeof(find_data_ptr->num_type));
                }
                if(valid_index_array != PNULL)
                {
                    //set number or mail valid flag
                    for(j =0; j < MMIPB_MAX_NV_PHONE_NUM; j++)
                    {
                        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[j].npi_ton, contact_ptr->number[j].number_len))
                        {
                            valid_index_array->visible_num_index[j] = 1;
                        }
                    }
#ifdef MMIPB_MAIL_SUPPORT
                    if(contact_ptr->mail.wstr_len > 0)
                    {
                        valid_index_array->is_mail_visible = TRUE;
                    }
#else
                    valid_index_array->is_mail_visible = FALSE;
#endif
                }
                result = MMIPB_ERROR_SUCCESS;
                break;
            }
        }
        else if(MMIPB_DailNumberContactCanbeEmpty(storage_id))
        {
            //无效记录也需要显示
            result = MMIPB_ERROR_SUCCESS;
            if(find_index != PNULL)
            {
                //set index
                *find_index = i;                   
            }
            if(find_data_ptr != PNULL)
            {
                //set find contact base information
                find_data_ptr->contact_id = access_point.contact.contact_id;
                find_data_ptr->storage_id = storage_id;
                find_data_ptr->index = i;
                find_data_ptr->group = (1<<(PB_GROUP_SIM_1 + MMIPB_GetDualSysByStorageId(storage_id)));
                find_data_ptr->visible_flag = 1;
                find_data_ptr->mail_exist = 0;//(contact_ptr->mail.wstr_len > 0 ? TRUE : FALSE);
                SCI_MEMSET(find_data_ptr->num_type, 0x00, sizeof(find_data_ptr->num_type));
            }
            break;
        }
    }
    SCI_FREE(contact_ptr);

    return result;
}

/*****************************************************************************/
//  Description : check current dail number contact can be empty
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DailNumberContactCanbeEmpty(uint16 storage_id)
{
     BOOLEAN result = FALSE;
#if defined (MMIPB_SIMDN_SUPPORT) 
     if(MMIPB_IsMsisdnContact(storage_id))
     {
         result = TRUE;
     }
#endif     
     return result;
}
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : Find visiable mostused contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindMostUsedContact(
                                               uint16 start_index,
                                               uint16 *find_index,//[OUT]                            
                                               MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 i = 0;
    //uint16 j = 0;
    
    //get a visibile contact from start_index
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER);
    if(s_pb_mostused_contact_list.contact_table != PNULL)
    {
        for(i = start_index; i < s_pb_mostused_contact_list.count; i++)
        {
            //if(s_pb_mostused_contact_list.visibility_table[i] > 0)
            {
                //find a visible contact
                if(find_index != PNULL)
                {
                    //set find index
                    *find_index = i; 
                }
                result = MMIPB_ERROR_SUCCESS;
                if(find_data_ptr != PNULL)
                {
                    *find_data_ptr = s_pb_mostused_contact_list.contact_table[i];
                }
                break;
            }
        }
    }
    //SCI_TRACE_LOW("MMIPB_FindMostUsedContact start_index %d   result %d",start_index, result);
    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p);
    //SCI_RestoreIRQ();
    return result;
}
#endif
/*****************************************************************************/
//  Description : 读记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactList(
                                           MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                                           )
{
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    *data_ptr = s_pb_contact_list.list_info;
    //SCI_RestoreIRQ();
    
    return MMIPB_ERROR_SUCCESS;
}
#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : FDN有效记录的个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFdnContactCount(uint16 storage_id)
{
    uint16 count = 0;
#ifdef FDN_SUPPORT
    MMIFDN_CONTEXT_T *fdn_context_ptr = MMIAPIFDN_GetFDNContext(MMIPB_GetDualSysByStorageId(storage_id));

    if(fdn_context_ptr != PNULL)
    {
        count = fdn_context_ptr->fdn_info.used_record_num;
    }
#endif
    return count;
}
#endif

/*****************************************************************************/
//  Description : sim卡特殊号码的个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSIMDNContactCount(uint16 storage_id)
{

    MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetDualSysByStorageId(storage_id));
    return storage_info.used_record_num;
}

/*****************************************************************************/
//  Description : 获取该联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactSimpleInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        data_ptr->group = s_pb_contact_list.list[index].group;//GetContactGroup(s_pb_contact_list.list[index]);
        data_ptr->contact_id = s_pb_contact_list.list[index].contact_id;
        data_ptr->first_char = s_pb_contact_list.list[index].first_char;
        data_ptr->index = s_pb_contact_list.list[index].index;
        data_ptr->storage_id = ShortIDToStorageID(s_pb_contact_list.list[index].short_storage_id);
        data_ptr->visible_flag = s_pb_contact_list.list[index].visible_flag;
        data_ptr->photo_id_flag = s_pb_contact_list.list[index].photo_id_flag;
        GetContactNumType(&(s_pb_contact_list.list[index]), data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
        data_ptr->mail_exist = IsContactHasMail(&(s_pb_contact_list.list[index]));
        result = MMIPB_ERROR_SUCCESS;
    }
    //SCI_RestoreIRQ();
    
    return result;
}

/*****************************************************************************/
//  Description : 获取手机联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadPhoneContactBaseInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16        i  = 0;
    uint16        phone_index  = 0;
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    for(i = 0; i < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL; i++)
    {
        if(MMIPB_IsPhoneContact(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
        {
            if(index == phone_index)
            {
                if(data_ptr != PNULL)
                {
                    data_ptr->group = s_pb_contact_list.list[i].group;//GetContactGroup(s_pb_contact_list.list[index]);
                    data_ptr->contact_id = s_pb_contact_list.list[i].contact_id;
                    data_ptr->first_char = 0;
                    data_ptr->index = index;
                    data_ptr->storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                    data_ptr->visible_flag = 0xFF;
                    data_ptr->photo_id_flag =  s_pb_contact_list.list[i].photo_id_flag;
                    if(index < s_pb_phone_contact_list.count_count && s_pb_phone_contact_list.contact_table != PNULL)
                    {
                        //GetContactNumType(&(s_pb_contact_list.list[index]), data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
                        SCI_MEMCPY(data_ptr->num_type, 
                            s_pb_phone_contact_list.contact_table[index].num_type, 
                            sizeof(uint8)*MMIPB_MAX_NV_PHONE_NUM);
                        result = MMIPB_ERROR_SUCCESS;

                    }
                   
#ifndef MMIPB_MEMPOOL_SUPPORT
                    else
                    {
                        MMIPB_CONTACT_T *contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
                        if(contact_ptr != PNULL)
                        {
                            MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
                            MMIPB_ACCESS_POINT_T access_point = {0};
                    
                            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                            access_point.contact.contact_id = s_pb_contact_list.list[i].contact_id;
                            access_point.contact.storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                            ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                                access_point,
                                sizeof(MMIPB_CONTACT_T),
                                contact_ptr
                                );
                            if(MMIPB_ERROR_SUCCESS == ret)
                            {
                                SCI_MEMCPY(data_ptr->num_type, contact_ptr->num_type, sizeof(uint8)*MMIPB_MAX_NV_PHONE_NUM);
                            }
                            SCI_FREE(contact_ptr);
                        }
                    }
#endif
                }
                break;
            }
            phone_index++;  
        }
              
    }
    return result;
}

#ifdef MMIPB_SYNC_WITH_O365
typedef enum
{
	MMIPB_SYBC_CONNECT_DISCONNECTED,
	MMIPB_SYBC_CONNECT_CONNECTING,
	MMIPB_SYBC_CONNECT_CONNECTED,
	MMIPB_SYBC_CONNECT_DISCONNECTING
}MMIPB_SYBC_CONNECT_STATE;

LOCAL MMIPB_SYBC_CONNECT_STATE s_bHttpState = MMIPB_SYBC_CONNECT_DISCONNECTED;
LOCAL uint32 s_nSynErrorCode = 0;
LOCAL BOOLEAN s_bShowEorr = FALSE;
LOCAL BOOLEAN s_bBackGroundSync = FALSE;
LOCAL BOOLEAN s_bStopSync = FALSE;
LOCAL BOOLEAN s_bHeartbeatStart = FALSE;
LOCAL MMIPB_SYNC_STOP_CALLBACK_FUNC s_pStopSyncCallback = PNULL;
LOCAL SCI_SEMAPHORE_PTR s_pb_local_update_list_semphore_p = PNULL;

LOCAL void MMIPB_SYNC_ClearPhoneContactCheckedFlag(void);
extern PUBLIC void contact_sync_get_token_cb( int result, char*  buf, int buflen);

PUBLIC BOOLEAN MMIPB_SYNC_IsHttpConnected(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsHttpConnected s_bHttpState = %d", s_bHttpState);
	if (MMIPB_SYBC_CONNECT_CONNECTED == s_bHttpState)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpConnected TRUE");
		return TRUE;
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpConnected FALSE");
	return FALSE;
}

PUBLIC BOOLEAN MMIPB_SYNC_IsHttpDisconnecting(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsHttpDisconnecting s_bHttpState = %d", s_bHttpState);
	if (MMIPB_SYBC_CONNECT_DISCONNECTING == s_bHttpState)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpDisconnecting TRUE");
		return TRUE;
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpDisconnecting FALSE");
	return FALSE;
}

PUBLIC BOOLEAN MMIPB_SYNC_IsHttpConnecting(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsHttpConnecting s_bHttpState = %d", s_bHttpState);
	if (MMIPB_SYBC_CONNECT_CONNECTING == s_bHttpState)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpConnecting TRUE");
		return TRUE;
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpConnecting FALSE");
	return FALSE;
}

PUBLIC BOOLEAN MMIPB_SYNC_IsHttpDisconnected(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsHttpDisconnected s_bHttpState = %d", s_bHttpState);
	if (MMIPB_SYBC_CONNECT_DISCONNECTED == s_bHttpState)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpDisconnected TRUE");
		return TRUE;
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsHttpDisconnected FALSE");
	return FALSE;
}

PUBLIC void MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_STATE httpState)
{
	s_bHttpState = httpState;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetHttpState s_bHttpReady = %d", s_bHttpState);
}

PUBLIC MMIPB_SYBC_CONNECT_STATE MMIPB_SYNC_GetHttpState(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_GetHttpState s_bHttpState = %d", s_bHttpState);
	return s_bHttpState;
}

PUBLIC BOOLEAN MMIPB_SYNC_IsSyncSuccessed(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_IsSyncSuccessed s_nSynErrorCode = %d", s_nSynErrorCode);

	if (s_nSynErrorCode == 0)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsSyncSuccessed TRUE");
		return TRUE;
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_IsSyncSuccessed FALSE");
	return FALSE;
}

PUBLIC uint32 MMIPB_SYNC_GetSyncErrorCode(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_GetSyncErrorCode s_nSynErrorCode = %d", s_nSynErrorCode);
	return s_nSynErrorCode;
}

PUBLIC void MMIPB_SYNC_SetSyncErrorCode(uint32 errorCode)
{
	s_nSynErrorCode = errorCode;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetSyncErrorCode s_nSynErrorCode = %d", s_nSynErrorCode);
}

PUBLIC BOOLEAN MMIPB_SYNC_IsSyncInBackGround(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsSyncInBackGround s_bBackGroundSync = %d", s_bBackGroundSync);
	return s_bBackGroundSync;
}

PUBLIC void MMIPB_SYNC_SetSyncInBackGround(BOOLEAN bBackGround)
{
	s_bBackGroundSync = bBackGround;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetSyncInBackGround s_bBackGroundSync = %d", s_bBackGroundSync);
}

PUBLIC BOOLEAN MMIPB_SYNC_IsStopSync(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsStopSync s_bStopSync = %d", s_bStopSync);
	return s_bStopSync;
}

//zouruoqi add for prompting error windows
PUBLIC void MMIPB_SYNC_SetUpToDate(BOOLEAN UpToDate)
{
	isUpToDate = UpToDate;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Uptodate UpToDate = %d", isUpToDate);
}

PUBLIC BOOLEAN MMIPB_SYNC_IsUpToDate(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Uptodate UpToDate = %d", isUpToDate);
	return isUpToDate;
}
PUBLIC void MMIPB_SYNC_SetShowEorr(BOOLEAN bShowEorr)
{
	s_bShowEorr = bShowEorr;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetShowEorr bShowEorr = %d", s_bShowEorr);
}

PUBLIC BOOLEAN MMIPB_SYNC_IsShowEorr(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsShowEorr s_bShowEorr = %d", s_bShowEorr);
	return s_bShowEorr;
}

PUBLIC void MMIPB_SYNC_SetHeartbeatStart(BOOLEAN bHeartbeatStart)
{
	s_bHeartbeatStart = bHeartbeatStart;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetHeartbeatStart s_bHeartbeatStart = %d", s_bHeartbeatStart);
}

PUBLIC BOOLEAN MMIPB_SYNC_IsHeartbeatStart(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsHeartbeatStart s_bHeartbeatStart = %d", s_bHeartbeatStart);
	return s_bHeartbeatStart;
}

//end zouruoqi
PUBLIC void MMIPB_SYNC_SetStopSync(BOOLEAN bStopSync)
{
	s_bStopSync = bStopSync;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetSyncInBackGround s_bBackGroundSync = %d", s_bStopSync);
}

PUBLIC MMIPB_SYNC_STOP_CALLBACK_FUNC MMIPB_SYNC_GetStopSyncCallBack(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_GetStopSyncCallBack s_pStopSyncCallback = 0X%X", s_pStopSyncCallback);
	return s_pStopSyncCallback;
}

PUBLIC void MMIPB_SYNC_SetStopSyncCallBack(MMIPB_SYNC_STOP_CALLBACK_FUNC callback)
{
	s_pStopSyncCallback = callback;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_GetStopSyncCallBack s_pStopSyncCallback = 0X%X", s_pStopSyncCallback);
}

LOCAL void MMIPB_SYNC_Create_LocalUpdateList_Locker(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_CreateLocalUpdateListLocker");
	s_pb_local_update_list_semphore_p = SCI_CreateSemaphore("s_pb_local_update_list_semphore_p", 1);
}

LOCAL void MMIPB_SYNC_Set_LocalUpdateList_Lock(void)
{
	SCI_GetSemaphore(s_pb_local_update_list_semphore_p, SCI_WAIT_FOREVER);
	
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Set_LocalUpdateList_Lock");
}

LOCAL void MMIPB_SYNC_Set_LocalUpdateList_Unlock(void)
{
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Set_LocalUpdateList_Lock");
	
	SCI_PutSemaphore(s_pb_local_update_list_semphore_p);
}

PUBLIC MMIPB_ERROR_E MMIPB_SYNC_SetContactsSyncType(
                                                MMIPB_SYNC_TYPE_E sync_type
                                                );

PUBLIC void MMIPB_SYNC_Initalize(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Initalize");

	MMIPB_SYNC_NV_Initalize();
	
	MMIPB_SYNC_InitDataList();  //初始化phone update list
	
	MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_SUSPEND);//同步挂起状态
	
	MMIPB_SYNC_SetSyncErrorCode(0);
	
	MMIPB_SYNC_SetSyncInBackGround(TRUE);
	
	MMIPB_SYNC_Create_LocalUpdateList_Locker();

	MMIPB_SYNC_SetStopSync(FALSE);
	
	MMIPB_SYNC_SetStopSyncCallBack(PNULL);
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Initalize");
}

PUBLIC BOOLEAN MMIPB_SYNC_IsSyncing(void)
{
	if (MMIPB_SYNC_TYPE_SUSPEND == MMIPB_SYNC_GetContactsSyncType())
	{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsSyncing FALSE");
		return FALSE;
	}
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsSyncing TRUE");

	return TRUE;
}

PUBLIC BOOLEAN MMIPB_SYNC_Start_FirstSync(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync");
    if (!MMIAPIPB_IsCanSyncNow())
    {
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync MMIAPIPB_IsCanSyncNow FALSE");
    return FALSE;
    }
	//zouruoqi add for prompting error windows
	MMIPB_SYNC_SetShowEorr(TRUE);	
	//zouruoqi end
	/* Check if data connection is on */
	if (MMICONNECTION_SETTING_GPRS_SWITCH_ON != MMIAPICONNECTION_GetGPRSSwitchStatus())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync, data connection is off, skip");
		return FALSE;
	}

	/* Check if it is syncing */
	if (MMIPB_SYNC_IsSyncing())
	{
		//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_IS_SYNCING, PNULL);
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync, IsSyncing, skip");
		return FALSE;
	}

	/* Check if it is http connecting/disconecting */
	if (!MMIPB_SYNC_IsHttpDisconnected())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync, http connecting/disconnecting, skip");
		return FALSE;
	}

	/* Reset items in NV */
	MMIPB_SYNC_NV_Reset_SyncTimeInfo();

	/* Set sync type */
	MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_FIRST_TIME);

	/* Clear Sync error code */
	MMIPB_SYNC_SetSyncErrorCode(0);

	if (MMIPB_SYNC_IsHttpConnected())
	{
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_START, 0);
	}
	else
	{
		MMIAPIPBSYNC_StartInitHttp(); //激活contact sync http会话
		MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_CONNECTING);
	}
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync");

	return TRUE;
}

PUBLIC BOOLEAN MMIPB_SYNC_Start_LightSync(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Start_LightSync");
	if (!MMIAPIPB_IsCanSyncNow())
    {
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Start_LightSync MMIAPIPB_IsCanSyncNow FALSE");
    return FALSE;
    }
	/* Check if data connection is on */
	if (MMICONNECTION_SETTING_GPRS_SWITCH_ON != MMIAPICONNECTION_GetGPRSSwitchStatus())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync, data connection is off, skip");
		return FALSE;
	}
	
	/* Check if it is syncing */
	if (MMIPB_SYNC_IsSyncing())
	{
		//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_IS_SYNCING, PNULL);
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_LightSync, IsSyncing, skip");
		return FALSE;
	}

	/* Check if it is http connecting/disconecting */
	if  (!MMIPB_SYNC_IsHttpDisconnected())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_LightSync, http connecting/disconnecting, skip");
		return FALSE;
	}

	/* Set sync type */
	MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_LIGHT);

	/* Clear Sync error code */
	MMIPB_SYNC_SetSyncErrorCode(0);

	if (MMIPB_SYNC_IsHttpConnected())
	{
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_START, 0);
	}
	else
	{
		MMIAPIPBSYNC_StartInitHttp(); //激活contact sync http会话
		MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_CONNECTING);
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_LightSync");

	return TRUE;
}

PUBLIC BOOLEAN MMIPB_SYNC_Start_HeavySync(BOOLEAN isBackGround)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Start_HeavySync");
	if (!MMIAPIPB_IsCanSyncNow())
    {
    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Start_HeavySync MMIAPIPB_IsCanSyncNow FALSE");
    return FALSE;
    }	
	//zouruoqi add for prompting error windows
	MMIPB_SYNC_SetShowEorr(TRUE);	
	//zouruoqi end
	/* Check if data connection is on */
	if (MMICONNECTION_SETTING_GPRS_SWITCH_ON != MMIAPICONNECTION_GetGPRSSwitchStatus())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_FirstSync, data connection is off, skip");
		return FALSE;
	}

	/* Check if it is syncing */
	if (MMIPB_SYNC_IsSyncing())
	{
		//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_IS_SYNCING, PNULL);
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_HeavySync, IsSyncing, skip");
		return FALSE;
	}

	/* Check if it is http connecting/disconecting */
	if  (!MMIPB_SYNC_IsHttpDisconnected())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_HeavySync, http connecting/disconnecting, skip");
		return FALSE;
	}

	/* Set sync type */
	MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_HEAVY);
    MMIPB_SYNC_ClearPhoneContactCheckedFlag();//shiwei mov here 20150909

	/* Clear Sync error code */
	MMIPB_SYNC_SetSyncErrorCode(0);

	/*Set background mode */
	MMIPB_SYNC_SetSyncInBackGround(isBackGround);

	if (MMIPB_SYNC_IsHttpConnected())
	{
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_START, 0);
	}
	else
	{
		MMIAPIPBSYNC_StartInitHttp(); //激活contact sync http会话
		MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_CONNECTING);
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start_HeavySync");

	return TRUE;
}

#if 0
PUBLIC BOOLEAN MMIPB_SYNC_Check_And_Start_FirstSync(void)
{
	BOOLEAN value = TRUE;
	return value;
}

PUBLIC BOOLEAN MMIPB_SYNC_NV_Is_HeartBeat_On(void)
{
	BOOLEAN value = TRUE;
	return value;
}

PUBLIC BOOLEAN MMIPB_SYNC_NV_Set_HeartBeat(BOOLEAN value)
{
	BOOLEAN ret = TRUE;
	return ret;
}
#endif

PUBLIC BOOLEAN MMIPB_SYNC_Stop_Sync(MMIPB_SYNC_STOP_CALLBACK_FUNC stop_call_back)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Stop_Sync");
	
	/* Check if it is syncing */
	if (FALSE == MMIPB_SYNC_IsSyncing())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Stop_Sync, Not Syncing, skip");
		return TRUE;
	}

	/* Check if it is already stopped sync */
	if (TRUE == MMIPB_SYNC_IsStopSync())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Stop_Sync, already stopped sync, skip");
		return TRUE;
	}

	/* Stop sync */
	MMIPB_SYNC_SetStopSyncCallBack(stop_call_back);
	MMIPB_SYNC_SetStopSync(TRUE);

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Stop_Sync");

	return TRUE;
}

PUBLIC BOOLEAN MMIPB_SYNC_Remove_All_AccountData(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Remove_All_AccountData");

	/* MSA Sign off */
	MMIPB_SYNC_NV_Set_MSA_Sign_In(FALSE);
	
    /* delete all pb */
	MMIAPIPB_DeleteAllOffice365Contacts();
    //MMIAPIPB_DeleteAllPBInfo();  

    /* reset pb sync NV */
    MMIPB_SYNC_NV_Reset_SyncTimeInfo();
	MMIPB_SYNC_NV_Set_HeartBeat(FALSE);
	MMIPB_SYNC_NV_Set_First_Sync(TRUE);

    /* delete local delete list file */
	MMIPB_SYNC_Del_All_Phone_Update_Item();

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Remove_All_AccountData");

	return TRUE;
}

PUBLIC void MMIPB_SYNC_Check_HeartBeat(void)
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
	//20150923,add by df
	static BOOLEAN bRetry = FALSE;
	//add end

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Check_HeartBeat");
	
	/* Get current time */
#if 0
	sys_time.min = 0;
	sys_time.hour = 12;
	//sys_date.wday = 0;
	sys_date.wday = 1;
#else
	TM_GetSysTime(&sys_time);
    TM_GetSysDate(&sys_date);
#endif

	/* Check if it is 12:00 */
	if (sys_time.min != 0 && (bRetry==FALSE) ) 
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Check_HeartBeat, min not match, skip");
		return;
	}
	
	if (sys_time.hour != 12) 
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Check_HeartBeat, hour not match, skip");
		return;
	}

	/* Check if heart beat on */
	if (FALSE == MMIPB_SYNC_NV_Is_HeartBeat_On())
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Check_HeartBeat, headbeat off, skip");
		return;
	}

	//20150923,add by dufei
	if (!MMIAPIPB_IsCanSyncNow()){
		bRetry =TRUE;
		return;
	}
	bRetry = FALSE;
	//add end
	
	/* Check if it is Sunday */
	if (sys_date.wday != 0) 
	{
		/* no, start a light sync */
		MMIPB_SYNC_Start_LightSync();
	}
	else
	{
		/* yes, start a heavy sync */
		if (TRUE == MMIPB_SYNC_Start_HeavySync(TRUE))
        {      
		MMIPB_SYNC_SetHeartbeatStart(TRUE);
         }
		//end
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Check_HeartBeat");
}

#if 0
/*****************************************************************************/
//  Description : 根据位置设置记录表中checked (Office365更新检查标识)
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-9
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_SetPhoneContactChecked(
                                               uint16 index, 
                                               BOOLEAN ischecked//[IN]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;

    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        s_pb_contact_list.list[index].checked = ischecked;
        result = MMIPB_ERROR_SUCCESS;
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    
    return result;
}
#endif

/****************************************************************************
**  Description : 通知更新列表
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/27/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_InformUpdateList(void)
{
    //MMI_HANDLE_T applet_handle = 0;
    //MMIPB_GetCurAppletHandle(&applet_handle);
    //MMIPB_UpdateCurrentList(applet_handle, MMIPB_OPC_ADD);  
    //MMK_SendMsg(applet_handle, MMI_PB_RELOAD_SEARCH_LIST_MSG, (ADD_DATA)TRUE);
	//PB_SYNC wangqinggang reload contact list
    MMIPB_RefreshEntryList();
	//PB_SYNC END

}


/*****************************************************************************/
//  Description : 清零记录表checked Flag
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-23
/*****************************************************************************/
LOCAL void MMIPB_SYNC_ClearPhoneContactCheckedFlag(void)
{
    //search postion in s_pb_contact_list
    uint32 i = 0;
    //SCI_TRACE_LOW("[PBSYNC] P--MMIPB_SYNC_ClearPhoneContactCheckedFlag SCI_GetSemaphore +++");
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            s_pb_contact_list.list[i].checked = FALSE;
        }
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    //SCI_TRACE_LOW("[PBSYNC] P--MMIPB_SYNC_ClearPhoneContactCheckedFlag SCI_PutSemaphore ---");
}

/*****************************************************************************/
//  Description : 根据o365 id 设置记录表中checked (Office365更新检查标识)
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-23
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_SetPhoneContactCheckedByO365Id(
                                               char *O365Id//[IN]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    BOOLEAN isfound = FALSE;
    uint32 i = 0;
    
    if(O365Id == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    
    if(s_pb_contact_list.list_info.count > s_pb_contact_list.max_count)
    {
        return result;
    }
    SCI_TRACE_LOW("[PBSYNC] P--MMIPB_SYNC_SetPhoneContactCheckedByO365Id SCI_GetSemaphore +++");
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    //search postion in s_pb_contact_list
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if((0 == MMIAPICOM_StrCmp((uint8 *)O365Id, SCI_STRLEN(O365Id), (uint8 *)&s_pb_contact_list.list[i].o365_id, SCI_STRLEN(s_pb_contact_list.list[i].o365_id)))
                && (FALSE == s_pb_contact_list.list[i].checked))
            {
                isfound = TRUE;
                //SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetPhoneContactCheckedByO365Id coming o365 len %d .", strlen(O365Id));
                //SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetPhoneContactCheckedByO365Id phone o365 len %d .", strlen(s_pb_contact_list.list[i].o365_id));
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetPhoneContactCheckedByO365Id found coming o365 id %s .", O365Id);
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetPhoneContactCheckedByO365Id found phone record o365 id %s .", s_pb_contact_list.list[i].o365_id);
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetPhoneContactCheckedByO365Id found contact id %d .", s_pb_contact_list.list[i].contact_id);
                s_pb_contact_list.list[i].checked = TRUE;
                break;
            }
        }
    }
    SCI_TRACE_LOW("[PBSYNC] P--MMIPB_SYNC_SetPhoneContactCheckedByO365Id SCI_PutSemaphore ---");
    SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return isfound;
}

#if 0
PUBLIC void MMIPB_SYNC_UpdateO365DeletedOrContinue(void)
{
    /* different sync type */
    if(MMIPB_SYNC_TYPE_LIGHT == cur_sync_type)//light sync
    {
        // go to next step
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
    }
    else if(MMIPB_SYNC_TYPE_HEAVY == cur_sync_type)//heavy sync
    {
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_ID_AND_MODIFIED_TIME, 0);
        MMIPB_SYNC_ClearPhoneContactCheckedFlag();//20150821
        MMIPB_SYNC_Phone_Update_List_Destroy();//destroy current phone update list 20150821
        MMISRV_PBSYNC_OpenWaitingWindow();
    }
    else
    {
        //error
    }
}
#endif

/*****************************************************************************/
//  Description : 根据o365 id 设置记录表中checked (Office365更新检查标识)
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-9
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateO365DeletedContactsToPB(void)
{
    uint16 storage_id = 0;
    uint16 i = 0;
    uint16 j = 0;
    uint16 delete_contact_id[MMIPB_NV_MAX_RECORD_NUM] = {0};

    SCI_TRACE_LOW("+[MMIPBSYNC]  MMIPB_SYNC_UpdateO365DeletedContactsToPB s_pb_contact_list.max_count = %d .", s_pb_contact_list.max_count);
    
    if(s_pb_contact_list.list_info.count > s_pb_contact_list.max_count)
    {
        SCI_TRACE_LOW("-[MMIPBSYNC] !!! MMIPB_SYNC_UpdateO365DeletedContactsToPB s_pb_contact_list.list_info.count = %d .", s_pb_contact_list.list_info.count);
        return MMIPB_ERROR_ERROR;
    }
#if 0
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if(FALSE == s_pb_contact_list.list[i].checked)//can test use true
            {
                storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateO365DeletedContactsToPB delete local contact first char is = %s .", s_pb_contact_list.list[i].first_char);
                if(MMIPB_ERROR_SUCCESS == MMIAPIPB_DeleteContact(s_pb_contact_list.list[i].contact_id, storage_id))//delete NV
                {
                    MMIPB_SYNC_InformUpdateList();
                    i--;//all contacts after this deleted one will move forward
                }
                else
                {
                    SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateO365DeletedContactsToPB contact_id = %d  fail.", s_pb_contact_list.list[i].contact_id);
                    //return MMIPB_ERROR_ERROR;
                }
            }
        }
    }
#endif
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if((FALSE == s_pb_contact_list.list[i].checked) &&
			   (0 != SCI_STRLEN(s_pb_contact_list.list[i].o365_id)))
            {
                delete_contact_id[j] = s_pb_contact_list.list[i].contact_id;
                SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetPhoneContactCheckedByO365Id find o365 deleted contact_id %d .", s_pb_contact_list.list[i].contact_id);
                j++;
            }
        }
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    //storage id is phone 
    storage_id = ShortIDToStorageID(s_pb_contact_list.list[0].short_storage_id);
    for(i= 0; i < j; i++)
    {
        if(MMIPB_ERROR_SUCCESS == MMIAPIPB_DeleteContact(delete_contact_id[i], storage_id))//delete NV
        {
            //MMIPB_SYNC_InformUpdateList();
            //provide contact info to pb_viewer
            MMIPB_SYNC_Notify_Update_Info(delete_contact_id[i], storage_id, MMIPB_OPC_DELETE);
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateO365DeletedContactsToPB local contact contact_id = %d SUCCESS.", delete_contact_id[i]);
        }
        else
        {
            SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateO365DeletedContactsToPB contact_id = %d  fail.", s_pb_contact_list.list[i].contact_id);
        }
    }
    //MMIPB_SYNC_ClearPhoneContactCheckedFlag();//clear for next heavy sync
    // next step
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdateO365DeletedContactsToPB");
    return MMIPB_ERROR_SUCCESS;
    //MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
    //SCI_PutSemaphore(s_contact_list_semphore_p);
}

#if 0
/*****************************************************************************/
//  Description : 根据位置从 在记录表中phone contact id
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-9
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_ReadPhoneContactId(
                                               uint16 index, 
                                               uint16 *data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        *data_ptr = s_pb_contact_list.list[index].contact_id;
        result = MMIPB_ERROR_SUCCESS;
    }
    //SCI_RestoreIRQ();
    
    return result;
}

/*****************************************************************************/
//  Description : 根据位置比较O365 修改时间
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : 1: new, 0: equal, -1: old 
//  Note: 2015-7-9
/*****************************************************************************/
PUBLIC int32 MMIPB_SYNC_CmpO365ModifiedTime(
                                               uint16 index, 
                                               uint32 cmpTime//[IN]
                                               )
{
    //return  MMIAPICOM_Stricmp((uint8 *)o365time_ptr, (uint8 *)&s_pb_contact_list.list[index].o365_modifiedTime);
    int32 result = 0;
    if (cmpTime > s_pb_contact_list.list[index].o365_modifiedTime)
    {
        return 1;
    }
    else if (cmpTime == s_pb_contact_list.list[index].o365_modifiedTime)
    {
        return 0;
    }
    return -1;
}

/*****************************************************************************/
//  Description : 根据Office365_id 在记录表中找到对应位置
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : TRUE: find this record pos in s_pb_contact_list, FALSE: not find in s_pb_contact_list    
//  Note: 2015-7-8
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_GetListNodePosByO365Contact(
                                                 MMIPB_CONTACT_T *contact_ptr,//[IN]
                                                 uint32 *pos//[OUT]
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    char *cmp_str = PNULL;
    BOOLEAN isfound = FALSE;
    uint32 i = 0;
    
    if(contact_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    
    if(s_pb_contact_list.list_info.count >= s_pb_contact_list.max_count)
    {
       return result;
    }
    cmp_str = contact_ptr->o365_id;
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 

    //search postion in s_pb_contact_list
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if(0 == MMIAPICOM_Stricmp((uint8 *)cmp_str, (uint8 *)&s_pb_contact_list.list[i].o365_id))
            {
                isfound = TRUE;
                *pos = i;
                break;
            }
        }
    }

    //SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return isfound;
}

/*****************************************************************************/
//  Description : 根据根据记录id和storage id来读取O365Id
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : TRUE: find this record and o365id exist, FALSE: not find the record or no o365id in this record    
//  Note: 2015-7-8
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_GetO365IdByPhoneId(
                                                uint16 contact_id,
                                                uint16 stoage_id,
                                                char *O365Id//[OUT]
                                                )
{
    uint16 i = 0;
    BOOLEAN isfound = FALSE;
    if(O365Id == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); ;
    if(contact_id > 0 && s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(contact_id == s_pb_contact_list.list[i].contact_id
                && stoage_id == ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id))
            {
                if(0 != SCI_STRLEN(s_pb_contact_list.list[i].o365_id))
                {//o365id  exist
                    SCI_MEMCPY(O365Id, s_pb_contact_list.list[i].o365_id, sizeof(s_pb_contact_list.list[i].o365_id));
                    isfound = TRUE;
                }
                break;
            }
        }
    }
    
    //SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return isfound;
}
//for test
PUBLIC void MMIPB_SYNC_SetCurNodeToHead(void)
{
    if ((PNULL == cur_pb_update_node) && (PNULL != MMIPB_SYNC_Get_Phone_Update_List_Head()))
    {
        cur_pb_update_node = MMIPB_SYNC_Get_Phone_Update_List_Head();
    }
}
#endif
/*****************************************************************************/
//  Description : 根据记录O365Id 读取contact_Id(phone)
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : 
//  Note: 2015-7-28
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetPhoneIdByO365Id(
                                                char *O365Id,//[IN]
                                                uint16 *contact_id,//[OUT]
                                                uint16 *stoage_id//[OUT]
                                                )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint32 i = 0;
    
    if(O365Id == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    
    if(s_pb_contact_list.list_info.count > s_pb_contact_list.max_count)
    {
       return MMIPB_ERROR_ERROR;
    }

    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    //search postion in s_pb_contact_list
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if(0 == MMIAPICOM_StrCmp((uint8 *)O365Id, SCI_STRLEN(O365Id), (uint8 *)&s_pb_contact_list.list[i].o365_id, SCI_STRLEN(s_pb_contact_list.list[i].o365_id)))    
            {
                *contact_id = s_pb_contact_list.list[i].contact_id;
                *stoage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                result = MMIPB_ERROR_SUCCESS;
                break;
            }
        }
    }

    //SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return result;
}


/*****************************************************************************/
//  Description : 将phone添加的所有联系人保存到PHONE_UPDATE_LIST
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :   
//  Note: 2015-7-21//did not test
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_PreparePhoneUpdateList(void)
{
#if 1
    uint16 i = 0;
    //MMIPB_OPC_E        modify_type = MMIPB_OPC_DELETE;
    uint16 stoage_id = 0;
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); ;
    if(s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(0 == SCI_STRLEN(s_pb_contact_list.list[i].o365_id))
            {//o365id not  exist, so this is Added contact.
                stoage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                MMIPB_SYNC_Phone_Update_List_Append_A(s_pb_contact_list.list[i].contact_id, stoage_id, s_pb_contact_list.list[i].phone_modifiedTime);
                SCI_TRACE_LOW("[PBSYNC] MMIPB_SYNC_PreparePhoneUpdateList add contact_id = %d into update list", s_pb_contact_list.list[i].contact_id);
            }
            else if(s_pb_contact_list.list[i].phone_modifiedTime != s_pb_contact_list.list[i].o365_modifiedTime)
            {//phone_modifiedTime  not  equal o365_modifiedTime, so this is a local modifed contact.
               MMIPB_SYNC_Phone_Update_List_Append(s_pb_contact_list.list[i].o365_id, 
					s_pb_contact_list.list[i].o365_modifiedTime, 
					s_pb_contact_list.list[i].phone_modifiedTime,
					MMIPB_OPC_EDIT);
               SCI_TRACE_LOW("[PBSYNC] MMIPB_SYNC_PreparePhoneUpdateList edit contact_id = %d into update list", s_pb_contact_list.list[i].contact_id);
            }
        }
    }

	//add phone del items to phone_update_list
	MMIPB_SYNC_Read_All_Phone_Update_Item();

	//initalize cur_pb_update_node
	//cur_pb_update_node = MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Head();
	MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Head();
	
	return MMIPB_ERROR_SUCCESS;
	#else
	MMIPB_SYNC_Read_All_Phone_Update_Item();
	cur_pb_update_node = MMIPB_SYNC_Get_Phone_Update_List_Head();
    MMIAPIPBSYNC_PB_DeleteContactHandler();
	return MMIPB_ERROR_SUCCESS;
	#endif
    
    //SCI_PutSemaphore(s_contact_list_semphore_p);
  
    //return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Check total count and added count for heavy sync
//  Global resource dependence : 
//  Author: wangkun
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_IsHeavySyncRequired(void)
{ 
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_IsHeavySyncRequired");
	
	SCI_TRACE_LOW(" [MMIPBSYNC] cur_total_count = %d", sync_time_info.cur_total_count);
	SCI_TRACE_LOW(" [MMIPBSYNC] last_total_count = %d", sync_time_info.last_total_count);
	SCI_TRACE_LOW(" [MMIPBSYNC] cur_added_count = %d", sync_time_info.cur_added_count);

#if 0
	return TRUE; //wk test//shiwei 
#else	
	if (sync_time_info.cur_total_count - sync_time_info.last_total_count < sync_time_info.cur_added_count)
	{
		SCI_TRACE_LOW("- [MMIPBSYNC] MMIPB_SYNC_IsHeavySyncRequired TRUE");
		return TRUE;
	}

	SCI_TRACE_LOW("- [MMIPBSYNC] MMIPB_SYNC_IsHeavySyncRequired FALSE");
	return FALSE;
#endif	
}

/*****************************************************************************/
//  Description : Continue heavy sync after light sync process
//  Global resource dependence : 
//  Author: wangkun
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC void MMIPB_SYNC_ContinueHeavySync(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_ContinueHeavySync");
	if(MMIPB_SYNC_TYPE_LIGHT == cur_sync_type)//light sync
	{
		// go to next step
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
		SCI_TRACE_LOW("[MMIPBSYNC] light sync, complete");
	}
	else if(MMIPB_SYNC_TYPE_HEAVY == cur_sync_type)//heavy sync
	{
		if (MMIPB_SYNC_IsHeavySyncRequired())
		{
            MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_ID_AND_MODIFIED_TIME, 0);
            //MMIPB_SYNC_ClearPhoneContactCheckedFlag();//20150821 shiwei change
            MMIPB_SYNC_Phone_Update_List_Destroy();//destroy current phone update list 20150821
            //MMISRV_PBSYNC_OpenWaitingWindow();
			
			SCI_TRACE_LOW("[MMIPBSYNC] continue heavy sync");
		}
		else
		{
			MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
			SCI_TRACE_LOW("[MMIPBSYNC] heavy sync, complete");
		}
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ContinueHeavySync");
}

/********************************************************************************
** Description: This function is used to update phone changes to office365                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/22/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetContactInfoByO365Id(
                                                MMIPB_CONTACT_T *contact_ptr,//[OUT]
                                                char *O365Id//[IN]
                                                );
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetContactInfoByPhoneId(
                                                uint16 contact_id,//[IN]
                                                uint16 stoage_id,//[IN]
                                                MMIPB_CONTACT_T *contact_ptr//[OUT]
                                                );
LOCAL MMIPB_ERROR_E MMIPB_SYNC_UpdateOnePBChangeToO365(MMIPB_SYNC_PHONE_UPDATE_LIST_T * p_local_update_node)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T *contact_ptr = PNULL;

    SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_UpdateOnePBChangeToO365, p_local_update_node=0X%X", p_local_update_node);

	/* check local update node */
    if (PNULL == p_local_update_node)
    {
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdateOnePBChangeToO365 NULL!");
        return result;
    }

	/* allocate memory */
    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if (PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdateOnePBChangeToO365 out of memory!");
        return result;
    }
    //MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_UPLOAD_PHONE_CHANGE_TO_SERVER, PNULL);
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));

	switch(p_local_update_node->pb_data_change.modify_type)
	{
		case MMIPB_OPC_ADD:
			if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetContactInfoByPhoneId(p_local_update_node->pb_data_change.contact_id, 
										p_local_update_node->pb_data_change.storage_id,
										contact_ptr))
			{
				
				result = MMIAPIPBSYNC_SVR_AddContact((uint32)p_local_update_node, contact_ptr);
			}
			break;
			
		case MMIPB_OPC_EDIT:
			if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetContactInfoByO365Id(contact_ptr, p_local_update_node->pb_data_change.o365_id))
			{
				result = MMIAPIPBSYNC_SVR_ModifyContact((uint32)p_local_update_node, p_local_update_node->pb_data_change.o365_id, contact_ptr);
			}
			break;
			
		case MMIPB_OPC_DELETE:
			result = MMIAPIPBSYNC_SVR_DeteteContact((uint32)p_local_update_node, p_local_update_node->pb_data_change.o365_id);
			break;
			
		default:
			SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateOnePBChangeToO365 invalid modify_type!");
			break;
	}
	
    SCI_FREE(contact_ptr);

    SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdateOnePBChangeToO365, result=%d", result);
	
    return result;
}

#if 0
PUBLIC void MMIPB_SYNC_UpdatePBChangeToO365Continue(void)
{
    cur_pb_update_node = cur_pb_update_node->next;
    if (PNULL != cur_pb_update_node)
    {
        //sync next item
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_PHONE_LIST_ITEM_TO_O365, 0);
    }
    else
    {
    	MMIPB_SYNC_ContinueHeavySync();
#if 0		
        if(MMIPB_SYNC_TYPE_LIGHT == cur_sync_type)//light sync
        {
            // go to next step
            MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
        }
        else if(MMIPB_SYNC_TYPE_HEAVY == cur_sync_type)//heavy sync
        {
        	if (MMIPB_SYNC_IsHeavySyncRequired())
    		{
    			MMIPB_SYNC_ClearPhoneContactCheckedFlag();
				
				MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_ID_AND_MODIFIED_TIME, 0);
    		}
			else
			{
				MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
			}
        }
#endif
    }
    
}
#endif

/********************************************************************************
** Description: This function is used to update phone changes to office365                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/22/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdatePBChangeToO365(void)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
	MMIPB_SYNC_PHONE_UPDATE_LIST_T *cur_update_node = PNULL;

    SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_UpdatePBChangeToO365");

	/* cur_update_node is used in pb task only, should be thead safety. */
	cur_update_node = MMIPB_SYNC_Phone_Update_List_Get_Cursor();

	/* check if all http task completed ? */
	if ((PNULL == cur_update_node) && 
		 (TRUE == MMIPB_SYNC_HTTP_Request_List_IsEmpty()))
	{
		MMIPB_SYNC_ContinueHeavySync();

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdatePBChangeToO365, all done!");

		return MMIPB_ERROR_SUCCESS;
	}
	
	for(;;)
	{
		/* check if it is completed ? */
		if (PNULL == cur_update_node)
		{
			SCI_TRACE_LOW(" [MMIPBSYNC] last node.");
			break;
		}

		/* sync one node to server */
		result = MMIPB_SYNC_UpdateOnePBChangeToO365(cur_update_node);

		/* http task is full */
		if ( MMIPB_ERROR_SUCCESS != result )
		{
			SCI_TRACE_LOW(" [MMIPBSYNC] http task is full.");
			break;
		}

		/* next one */
		cur_update_node = MMIPB_SYNC_Phone_Update_List_Cursor_Goto_Next();
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdatePBChangeToO365");

    return MMIPB_ERROR_SUCCESS;
}

/********************************************************************************
** Description: This function is used to update added phone changes to office365                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/28/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateAddedContactFromO365(MMIPB_SYNC_PHONE_UPDATE_LIST_T * p_local_node, char* o365_id, uint32 uiTime)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T *contact_ptr = PNULL;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365, p_local_node=0X%X", p_local_node);

	/* check parameter */
    if ((PNULL == p_local_node) || (PNULL == o365_id))
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365 o365_id = %s.", o365_id);
        return MMIPB_ERROR_INVALID_PARAM;
    }

	/* check modify_type */
    if (MMIPB_OPC_ADD != p_local_node->pb_data_change.modify_type)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365 modify_type = %d  fail.", p_local_node->pb_data_change.modify_type);
        return MMIPB_ERROR_INVALID_FORMAT;
    }

	/* allocate memory */
    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if (PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365 out of memory!");
        return MMIPB_ERROR_INVALID_PARAM;
    }
	
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));

	/* lock local update list */
	//MMIPB_SYNC_Set_LocalUpdateList_Lock();

    /* update phone record */
    if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetContactInfoByPhoneId(p_local_node->pb_data_change.contact_id, 
                                            p_local_node->pb_data_change.storage_id,
                                            contact_ptr))
	{
		/* update o365 modify time to phone */
		contact_ptr->o365_modifiedTime = uiTime;
		
		/* update o365id to phone */
		SCI_MEMCPY(contact_ptr->o365_id, o365_id, sizeof(contact_ptr->o365_id));
		SCI_TRACE_LOW("[PBSYNC] ## O--->P MMIPB_SYNC_UpdateAddedContactFromO365 o365_id %s request", contact_ptr->o365_id);
		SCI_TRACE_LOW("[PBSYNC] ## O--->P MMIPB_SYNC_UpdateAddedContactFromO365 name %s request", contact_ptr->name.wstr);

		/* update NV */
		if(MMIPB_ERROR_SUCCESS == MMIPB_ModifyContact(contact_ptr))
		{
			result = MMIPB_ERROR_SUCCESS;
			SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365 success");
		}
		else
		{
			SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365 modify contact_id = %d  fail.", p_local_node->pb_data_change.contact_id);
		}
	}
	else
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365 get contact_id = %d info fail.", p_local_node->pb_data_change.contact_id);
    }

	/* un-lock local update list */
	//MMIPB_SYNC_Set_LocalUpdateList_Unlock();

	/* release memory */
    SCI_FREE(contact_ptr);

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdateAddedContactFromO365, result=%d", result);
    
    return result;
}

/********************************************************************************
** Description: This function is used to update modified phone changes to office365                          *
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Input:
** Note: 07/28/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateModifiedContactFromO365(MMIPB_SYNC_PHONE_UPDATE_LIST_T * p_local_node, char* o365_id, uint32 uiTime)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T *contact_ptr = PNULL;

	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365, p_local_node=0X%X", p_local_node);

	/* check parameter */
    if ((PNULL == p_local_node) || (PNULL == o365_id))
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365 o365_id = %d.", o365_id);
        return MMIPB_ERROR_INVALID_PARAM;
    }

	/* check modify_type */
    if (MMIPB_OPC_EDIT != p_local_node->pb_data_change.modify_type)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365 modify_type = %d  fail.", p_local_node->pb_data_change.modify_type);
        return MMIPB_ERROR_INVALID_FORMAT;
    }

	/* allocate memory */
    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if (PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365 out of memory!");
        return MMIPB_ERROR_INVALID_PARAM;
    }
	
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));

	/* lock local update list */
	//MMIPB_SYNC_Set_LocalUpdateList_Lock();

    /* update phone record */
    if (MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetContactInfoByO365Id(contact_ptr, p_local_node->pb_data_change.o365_id))
	{
		/* update o365 modify time to phone */
		contact_ptr->o365_modifiedTime = uiTime;
		
		//SCI_MEMCPY(contact_ptr->o365_id, o365_id, sizeof(contact_ptr->o365_id));

		/* update NV */
		if(MMIPB_ERROR_SUCCESS == MMIPB_ModifyContact(contact_ptr))
		{
			result = MMIPB_ERROR_SUCCESS;
			SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365 success");
		}
		else
		{
			SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365 MMIPB_ModifyContact fail.");
		}		
	}
	else
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365 get contact info by o365id = %d  fail.", p_local_node->pb_data_change.o365_id);
    }

	/* un-lock local update list */
	//MMIPB_SYNC_Set_LocalUpdateList_Unlock();

	/* release memory */
    SCI_FREE(contact_ptr);

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_UpdateModifiedContactFromO365, result=%d", result);

	return result;
}

/*****************************************************************************/
//  Description : re-add local phone modified item
//  Global resource dependence : 
//  Author: wangkun
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetListNodeByO365Id(
                                                MMIPB_CONTACT_LIST_NODE_T **node,//[OUT]
                                                char *O365Id//[IN]
                                                 );

PUBLIC MMIPB_ERROR_E MMIPB_SYNC_ReAddLocalPhoneModifyItem(MMIPB_SYNC_PHONE_UPDATE_LIST_T * p_local_node)
{
    MMIPB_CONTACT_LIST_NODE_T *pnode = PNULL;
	MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_ReAddLocalPhoneModifyItem");
	
	/* check curretn pb update node */
	if (NULL == p_local_node)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ReAddLocalPhoneModifyItem, p_local_node is null");
		return result;
	}

	if (MMIPB_OPC_EDIT != p_local_node->pb_data_change.modify_type)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ReAddLocalPhoneModifyItem, wrong modify_type");
		return result;
	}

	/* lock local update list */
	//MMIPB_SYNC_Set_LocalUpdateList_Lock();

    if(MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetListNodeByO365Id(&pnode, p_local_node->pb_data_change.o365_id))
    {
        uint16 short_storage_id = ShortIDToStorageID(pnode->short_storage_id);
		
        SCI_TRACE_LOW(" [MMIPBSYNC] re-add, contact_id = %d", pnode->contact_id);
        SCI_TRACE_LOW(" [MMIPBSYNC] re-add, short_storage_id = %d", short_storage_id);
        SCI_TRACE_LOW(" [MMIPBSYNC] re-add, phone_modifiedTime = %d", p_local_node->pb_data_change.phone_modifiedTime);
        MMIPB_SYNC_Phone_Update_List_Append_A(pnode->contact_id, short_storage_id, p_local_node->pb_data_change.phone_modifiedTime);
        result = MMIPB_ERROR_SUCCESS;
    }

	/* un-lock local update list */
	//MMIPB_SYNC_Set_LocalUpdateList_Unlock();
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ReAddLocalPhoneModifyItem, result=%d", result);
	
	return result;

}

/*****************************************************************************/
//  Description : clear the phone delete record after sync with the server
//  Global resource dependence : 
//  Author: wangkun
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_ClearLocalPhoneDeleteItem(MMIPB_SYNC_PHONE_UPDATE_LIST_T * p_local_node)
{
	MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
	
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_ClearLocalPhoneDeleteItem");
	
	/* check curretn pb update node */
	if (NULL == p_local_node)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ClearLocalPhoneDeleteItem, p_local_node is null");
		return MMIPB_ERROR_ERROR;
	}

	if (p_local_node->pb_data_change.modify_type != MMIPB_OPC_DELETE)
	{
		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ClearLocalPhoneDeleteItem, wrong modify_type");
		return MMIPB_ERROR_ERROR;
	}

	/* It is safe to be  called in mult-threads */
	result = MMIPB_SYNC_Del_Phone_Update_Item(p_local_node->pb_data_change.contact_id);
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_ClearLocalPhoneDeleteItem, result=%d", result);

	return result;
}

#if 0

/*****************************************************************************/
//  Description : 开始轻同步
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-23
/*****************************************************************************/
PUBLIC void MMIPB_SYNC_StartLightWeightContactsSync(void)
{
    cur_pb_update_node = MMIPB_SYNC_Get_Phone_Update_List_Head();
    //start to send request
    // (1) Create contact on the phone according to Office365 
    
    //use send signal to task MMIPB_SendSignalToPBTask(MMIPB_SIG_DELETE, s_mmipb_current_handle);
    MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_ADDED_TO_PHONE, 0);
}

/*****************************************************************************/
//  Description : 查询office365数量变化，判断是否需要重同步
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC void MMIPB_SYNC_GetHeavySyncNumbers(void)
{
     //MMIAPIPBSYNC_SVR_GetTotalCount(void);-----heavy sync check Step 1
}

/*****************************************************************************/
//  Description : 检查是否满足重同步条件(office365 删除了联系人)
//  如果满足，启动重同步流程
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC void MMIPB_SYNC_StartHeavyWeightContactsSync(uint16 added_count)
{
    //MMIAPIPBSYNC_SVR_GetTotalCount();
    if (sync_time_info.cur_total_count < sync_time_info.last_total_count)
    {
        // office365 had deleted contact.
        MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_HEAVY);
    }
    else if (sync_time_info.cur_total_count - sync_time_info.last_total_count == added_count)
    {
        // office365 had not deleted contact.
        // only do light weight sync
        MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_LIGHT);
    }
    else
    {
        //error
    }
    MMIPB_SYNC_StartLightWeightContactsSync();//-----heavy sync check Step 4---log
    sync_time_info.last_total_count = sync_time_info.cur_total_count;//current is recorded as last
}
#endif

/*****************************************************************************/
//  Description : 更新当前新加联系人总数
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateCurO365AddedContactsCount(
                                                uint16 added_contacts_num
                                                )
{      
    sync_time_info.cur_added_count = added_contacts_num;
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 更新当前修改联系人总数
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateCurO365UpdatedContactsCount(
                                                uint16 updated_contacts_num
                                                )
{      
    sync_time_info.cur_updated_count = updated_contacts_num;
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 根据phone_id 在记录表中找到对应节点
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : TRUE: find this record node in s_pb_contact_list, FALSE: not find in s_pb_contact_list    
//  Note: 2015-7-8
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SYNC_GetListNodeByPhoneId(
                                                uint16 contact_id,//[IN]
                                                uint16 stoage_id,//[IN]
                                                MMIPB_CONTACT_LIST_NODE_T **node//[OUT]
                                                 )
{
    uint16 i = 0;
    BOOLEAN result = FALSE;

    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); ;
    if(contact_id > 0 && s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {//search postion in s_pb_contact_list
            if(contact_id == s_pb_contact_list.list[i].contact_id
                && stoage_id == ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id))
            {
                *node = &s_pb_contact_list.list[i];
                result = TRUE;
                break;
            }
        }
    }
    
    //SCI_PutSemaphore(s_contact_list_semphore_p);
   
    return result;
}

/*****************************************************************************/
//  Description : 根据Office365_id 在记录表中找到对应节点
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : 
//  Note: 2015-7-28
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetListNodeByO365Id(
                                                MMIPB_CONTACT_LIST_NODE_T **node,//[OUT]
                                                char *O365Id//[IN]
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint32 i = 0;
    
    if(O365Id == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    
    if(s_pb_contact_list.list_info.count > s_pb_contact_list.max_count)
    {
        SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_GetListNodePosByO365Id max_count %d, count %d", s_pb_contact_list.max_count, s_pb_contact_list.list_info.count);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1296_112_2_18_2_43_21_180,(uint8*)"dd", s_pb_contact_list.max_count, s_pb_contact_list.list_info.count);
        return result;
    }

    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
    //search postion in s_pb_contact_list
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if(0 == MMIAPICOM_StrCmp((uint8 *)O365Id, SCI_STRLEN(O365Id), (uint8 *)&s_pb_contact_list.list[i].o365_id, SCI_STRLEN(s_pb_contact_list.list[i].o365_id)))
            {
                *node = &s_pb_contact_list.list[i];
                result = MMIPB_ERROR_SUCCESS;
                break;
            }
        }
    }

    //SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return result;
}

/*****************************************************************************/
//  Description : 根据根据office365id 读取联系人详细信息
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : TRUE: find this record and o365id exist, FALSE: not find the record or no o365id in this record    
//  Note: 2015-7-8
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetContactInfoByO365Id(
                                                MMIPB_CONTACT_T *contact_ptr,//[OUT]
                                                char *O365Id//[IN]
                                                )
{      
    MMIPB_CONTACT_LIST_NODE_T *pnode = PNULL;
    MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
    MMIPB_ACCESS_POINT_T access_point = {0};
    if(PNULL == contact_ptr)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    if(MMIPB_ERROR_SUCCESS == MMIPB_SYNC_GetListNodeByO365Id(&pnode, O365Id))
    {
        access_point.contact.contact_id = pnode->contact_id;
        access_point.contact.storage_id =ShortIDToStorageID(pnode->short_storage_id);
        ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
            access_point,
            sizeof(MMIPB_CONTACT_T),
            contact_ptr
            );
    }
    return ret;
}

/*****************************************************************************/
//  Description : 根据根据phone id 读取联系人详细信息
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return : TRUE: find this record and o365id exist, FALSE: not find the record or no o365id in this record    
//  Note: 2015-7-8
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_GetContactInfoByPhoneId(
                                                uint16 contact_id,//[IN]
                                                uint16 stoage_id,//[IN]
                                                MMIPB_CONTACT_T *contact_ptr//[OUT]
                                                )
{      
    MMIPB_CONTACT_LIST_NODE_T *pnode = PNULL;
    MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
    MMIPB_ACCESS_POINT_T access_point = {0};
    if(PNULL == contact_ptr)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    if(TRUE == MMIPB_SYNC_GetListNodeByPhoneId(contact_id, stoage_id, &pnode))
    {
        access_point.contact.contact_id = pnode->contact_id;
        access_point.contact.storage_id =ShortIDToStorageID(pnode->short_storage_id);
        ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,
            access_point,
            sizeof(MMIPB_CONTACT_T),
            contact_ptr
            );
    }
    return ret;
}

/*****************************************************************************/
//  Description : 设置当前同步类型
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_SetContactsSyncType(
                                                MMIPB_SYNC_TYPE_E sync_type
                                                )
{      
    cur_sync_type = sync_type;
	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_SetContactsSyncType,cur_sync_type=%d", cur_sync_type);
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 读取当前同步类型
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :  
//  Note: 2015-7-27
/*****************************************************************************/
PUBLIC MMIPB_SYNC_TYPE_E MMIPB_SYNC_GetContactsSyncType(void)
{      
    return cur_sync_type;
}

/*****************************************************************************/
//  Description : 更新Last Sync Time
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateCurSyncTime(
                                                uint32 sync_time
                                                )
{      
    if (sync_time > sync_time_info.cur_sync_time)
    {
    	//wangkun, search time should bigger than the last modified time ???
        sync_time_info.cur_sync_time = sync_time;
        //sync_time_info.cur_sync_time = sync_time + 1;
    }
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 更新当前联系人总数
//  Global resource dependence : 
//  Author: shiwei.zhang
//  return :  
//  Note: 2015-7-24
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_UpdateCurO365ContactsCount(
                                                uint16 contacts_num
                                                )
{      
    sync_time_info.cur_total_count = contacts_num;
    return MMIPB_ERROR_SUCCESS;
}

PUBLIC void MMIPB_SYNC_GetAllAddedContacts(void)
{//sync_time_info.last_light_sync_time = 1124436628;//test
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetAllAddedContacts, last_light_sync_time=%d", sync_time_info.last_light_sync_time);
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_ALL_ADDED_CONTACTS, PNULL);
	MMIAPIPBSYNC_SVR_GetAllAddedContacts(sync_time_info.last_light_sync_time);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetAllAddedContacts");
}

PUBLIC void MMIPB_SYNC_GetAllUpdatedContacts(void)
{//sync_time_info.last_light_sync_time = 1124436628;//test
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetAllUpdatedContacts, last_light_sync_time=%d", sync_time_info.last_light_sync_time);
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_ALL_UPDATED_CONTACTS, PNULL);
	MMIAPIPBSYNC_SVR_GetAllUpdatedContacts(sync_time_info.last_light_sync_time);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetAllUpdatedContacts");
}

PUBLIC void MMIPB_SYNC_GetTotalContactsCount(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetTotalContactsCount");
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_TOTAL_CONTACTS_COUNT, PNULL);
	MMIAPIPBSYNC_SVR_GetTotalCount();  //heavy sync check Step 1
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetTotalContactsCount");
//test
//  SCI_TRACE_LOW("-[MMIPBSYNC] MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount----1111111");
//  MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount(1111111);
//test
}

PUBLIC void MMIPB_SYNC_GetAllAddedContactsCount(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetAllAddedContactsCount, last_heavy_sync_time=%d", sync_time_info.last_heavy_sync_time);
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_ALL_ADDED_CONTACTS_COUNT, PNULL);
	MMIAPIPBSYNC_SVR_GetAllAddedContactsCount(sync_time_info.last_heavy_sync_time);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetAllAddedContactsCount");
}

PUBLIC void MMIPB_SYNC_GetAllUpdatedContactsCount(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetAllUpdatedContactsCount, last_heavy_sync_time=%d", sync_time_info.last_heavy_sync_time);
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_ALL_UPDATED_CONTACTS_COUNT, PNULL);
	MMIAPIPBSYNC_SVR_GetAllUpdatedContactsCount(sync_time_info.last_heavy_sync_time);
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetAllUpdatedContactsCount");
}

PUBLIC void MMIPB_SYNC_CheckIfHeavySyncPossible(void)
{
	MMIPB_SYNC_SetUpToDate(TRUE);
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_CheckIfHeavySyncPossible");

#ifdef MMIPB_SYNC_GET_COUNT_PARALLEL
    /* check if all http task completed ? */
    if (FALSE == MMIPB_SYNC_HTTP_Request_List_IsEmpty())
    {
        SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_CheckIfHeavySyncPossible, not all done yet!");
		MMIPB_SYNC_SetUpToDate(FALSE);
        return;
    }    
#endif

	if(TRUE == MMIPB_SYNC_IsSyncInBackGround())
	{
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_CheckIfHeavySyncPossible, IsSyncInBackGround");
		MMIPB_SYNC_HeavySyncConfirmed();
		MMIPB_SYNC_SetUpToDate(FALSE);
		return;
	}

	//Check if user has already canceled the dialog?
	if (MMIPB_IsSyncCheckWaitDialogCanceled())
	{
		SCI_TRACE_LOW("[MMIPBSYNC] heavy sync complete, user canceled.");		
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);

		SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_CheckIfHeavySyncPossible, use canceled.");
		return;
	}

	//close sync check dialog
	MMIPB_CloseSyncCheckWaitDialog();
	
	//check if it is required to show user confirm dialog for heavy sync
	if (FALSE == MMIPB_SYNC_Is_Phone_Update_List_Empty()) //phone add/modify/delete
	{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Is_Phone_Update_List_Empty is false");
		MMIPB_SYNC_SetUpToDate(FALSE);
	}
	else if (MMIPB_SYNC_IsHeavySyncRequired())  //or server delete
	{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_IsHeavySyncRequired is true");
		MMIPB_SYNC_SetUpToDate(FALSE);
	}
	else if (sync_time_info.cur_updated_count > 0) //or server modify
	{
		SCI_TRACE_LOW("[MMIPBSYNC] cur_updated_count =%d", sync_time_info.cur_updated_count);
		MMIPB_SYNC_SetUpToDate(FALSE);
	}
	
	else if (sync_time_info.cur_added_count > 0) //or server add
	{
			 SCI_TRACE_LOW("[MMIPBSYNC] cur_added_count =%d", sync_time_info.cur_added_count);
			 MMIPB_SYNC_SetUpToDate(FALSE);
	}

	if (TRUE == MMIPB_SYNC_IsUpToDate())
	{
		SCI_TRACE_LOW("[MMIPBSYNC] show up to date dialog");
		MMIPB_ShowUpToDateDialog();
		
		SCI_TRACE_LOW("[MMIPBSYNC] heavy sync complete, already up to date.");		
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);

	}
	else
	{
	    
		//-----heavy sync Step 5, show user confirm dialog
		SCI_TRACE_LOW("[MMIPBSYNC] show user confirm dialog for heavy sync");
		MMIPB_OpenSyncConfirmDialogWin();
	}

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_CheckIfHeavySyncPossible");
}

PUBLIC void MMIPB_SYNC_HeavySyncConfirmed(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_HeavySyncConfirmed");
	
	MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_ADDED_TO_PHONE, 0); //-----heavy sync Step 6, user confirmed heavy sync
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_HeavySyncConfirmed");
}

PUBLIC void MMIPB_SYNC_HeavySyncCanceled(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_HeavySyncCanceled");
	
	MMIPB_SendSignalToPBTask(MMIPB_SYNC_COMPLETE, 0);
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_HeavySyncCanceled");
}

PUBLIC void MMIPB_SYNC_GetAllIdAndModiTime(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetAllIdAndModiTime");
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_ALL_ID_AND_MODIFY_TIME, PNULL);
	MMIAPIPBSYNC_SVR_GetAllIdAndModiTime();
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetAllIdAndModiTime");
}

PUBLIC void MMIPB_SYNC_GetAllContacts(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_GetAllContacts");
	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_GET_ALL_CONTACTS, PNULL);
	MMIAPIPBSYNC_SVR_GetAllContacts();
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_GetAllContacts");
}

/****************************************************************************
**  Description : 同步开始
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/27/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_Start(void)
{
    SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Start");
	
    /* Read Sync time info from NV */
    MMIPB_SYNC_NV_Read_SyncTimeInfo(&sync_time_info);

	/* Create the phone update list */
    MMIPB_SYNC_PreparePhoneUpdateList();

	/* Start Sync */
	if (MMIPB_SYNC_TYPE_LIGHT == MMIPB_SYNC_GetContactsSyncType())
	{		
		//MMISRV_PBSYNC_OpenWaitingWindow(); //test
		
		//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_LIGHT_SYNCING, PNULL);
		
		SCI_TRACE_LOW("[MMIPBSYNC] light sync");
		
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_ADDED_TO_PHONE, 0);
	}
	else if (MMIPB_SYNC_TYPE_HEAVY == MMIPB_SYNC_GetContactsSyncType())
	{		
		//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_HEAVY_SYNCING, PNULL);
		
		SCI_TRACE_LOW("[MMIPBSYNC] heavy sync");
		
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_TOTAL_CONTACTS_COUNT, 0);
#ifdef MMIPB_SYNC_GET_COUNT_PARALLEL
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_ADDED_CONTACTS_COUNT, 0);
        MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_UPDATED_CONTACTS_COUNT, 0);
#endif
		//MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_ADDED_TO_PHONE, 0);
	}
	else
	{
		//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_FIRST_SYNCING, PNULL);
		
		SCI_TRACE_LOW("[MMIPBSYNC] first sync");
		
		MMIPB_SendSignalToPBTask(MMIPB_SYNC_TOTAL_CONTACTS_COUNT, 0);
		//MMIPB_SendSignalToPBTask(MMIPB_SYNC_ALL_O365_CONTACTS_TO_PHONE, 0);
	}
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Start");
}

/****************************************************************************
**  Description : 同步完成
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/27/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_Complete()
{
//zouruoqi 20160324 PBSYNC error windows
    MMIPB_CNF_MSG_T     cnf_msg = {0};
//zouruoqi
	MMIPB_SYNC_STOP_CALLBACK_FUNC stopsync_callback;
	uint32 ErrorCode = MMIPB_SYNC_GetSyncErrorCode();
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Complete %d", isUpToDate);
	//zouruoqi add for prompting error windows
	
	if (ErrorCode != RESPONSE_SUCCESS)
		{
			if(FALSE == MMIPB_SYNC_IsHeartbeatStart() && TRUE == MMIPB_SYNC_IsShowEorr())
		    {
		    //zouruoqi 20160324 PBSYNC error windows
		    	SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Complete MMIPB_CloseSyncResultDialog() networkerror");
			    MMIPB_SendSignalToMMITask(MMIPB_CloseSyncResultwin , &cnf_msg);
			    //cnf_msg.ret = MMIPB_CloseSyncResultDialog();          
				if(ErrorCode == RESPONSE_NOT_PERMITTED || ErrorCode == RESPONSE_NO_CHANCE_RETRY_ERROR )//error_code is RESPONSE_NOT_PERMITTED
				{
				//cnf_msg.handle = pb_signal->handle;
				//cnf_msg.handle_node = pb_signal->handle_node;
				SCI_TRACE_LOW("[MMIPBSYNC]SendSignal_ERROR_PERMISSION_OPEN_WIN");
				MMIPB_SendSignalToMMITask(MMI_PB_ERROR_PERMISSION_OPEN_WIN , &cnf_msg);
				MMIPB_SYNC_NV_Set_HeartBeat(FALSE);
				}
				else{
				SCI_TRACE_LOW("[MMIPBSYNC]SendSignal_ERROR_NETWORK_OPEN_WIN");
				MMIPB_SendSignalToMMITask(MMI_PB_ERROR_NETWORK_OPEN_WIN , &cnf_msg);
				
				}
				MMIPB_SYNC_SetShowEorr(FALSE);
	         }
			//zouruoqi
		}
        //end zouruoqi*/
		if(MMIPB_SYNC_IsUpToDate() == FALSE)//heavy sync
		{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Complete MMIPB_CloseSyncResultDialog() not uptodate");
			MMIPB_SendSignalToMMITask(MMIPB_CloseSyncResultwin , &cnf_msg);
		}

	if (MMIPB_SYNC_IsSyncSuccessed() && 
		(FALSE == MMIPB_SYNC_IsStopSync()))
	{
		/* set last sync time */
		if(MMIPB_SYNC_TYPE_LIGHT == cur_sync_type)//light sync
		{
			sync_time_info.last_light_sync_time = sync_time_info.cur_sync_time;
		}
#if 0	
		else if(MMIPB_SYNC_TYPE_HEAVY == cur_sync_type)//heavy sync
		{
			sync_time_info.last_heavy_sync_time = sync_time_info.cur_sync_time;
			
			sync_time_info.last_total_count = sync_time_info.cur_total_count;
		}
#endif	
		else //first sync
		{
			sync_time_info.last_heavy_sync_time = sync_time_info.cur_sync_time;
			sync_time_info.last_light_sync_time = sync_time_info.cur_sync_time;

			sync_time_info.last_total_count = sync_time_info.cur_total_count;
		}

		/* save sync time and count to NV */
		MMIPB_SYNC_NV_Write_SyncTimeInfo(&sync_time_info);

		if(MMIPB_SYNC_TYPE_FIRST_TIME == cur_sync_type) //first sync success
		{
			/* Set first enter flag */
			//MMIPB_SYNC_NV_Set_First_Sync(FALSE);
            MMIPB_SYNC_NV_Set_HeartBeat(TRUE);

			/* Set heartbeat on after first sync completed */
            
		}
	}

    MMIPB_SYNC_Phone_Update_List_Destroy();//destroy current phone update list
    MMIPB_SYNC_SetContactsSyncType(MMIPB_SYNC_TYPE_SUSPEND);
    //????? MMIPB_SYNC_InformUpdateList();//shiwei added 20150923
    //setting menu update has been set to logout()
    //MMIPB_UpdatePBSyncSettingMenu();

#if 0
	MMISRV_PBSYNC_CloseWaitingWindow();

	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_SYNC_DONE, PNULL);
#endif

	/* deactive http connect */
	if(MMIPB_SYNC_IsHttpConnected())
	{
		MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_DISCONNECTING);
		MMIAPIPBSYNC_StopHttp();
	}

	/* check stop sync */
	if (MMIPB_SYNC_IsStopSync())
	{
		stopsync_callback = MMIPB_SYNC_GetStopSyncCallBack();
		if (PNULL != stopsync_callback)
		{
			stopsync_callback();
		}

		MMIPB_SYNC_SetStopSyncCallBack(PNULL);

		MMIPB_SYNC_SetStopSync(FALSE);		
	}
	//zouruoqi add for prompting error windows
	MMIPB_SYNC_SetHeartbeatStart(FALSE);
	//end
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Complete sync type = %d ",MMIPB_SYNC_GetContactsSyncType());
}

/*****************************************************************************/
//  Description : get token cb func
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SYNC_Get_Token_Callback( int result, char*  buf, uint16 buflen)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Get_Token_Callback,result=%d", result);
	if(MSA_OK == result)
	{
		contact_sync_get_token_cb(result, buf, buflen);
		
		MMIAPIMSA_Exit();
	}
	else if(MSA_CANCEL == result)
	{
		SCI_TRACE_LOW("[MMIPBSYNC] MMIPB_SYNC_Get_Token_Callback error!");
		/* do nothing ?? */
	}

	MMIPB_SendSignalToPBTask(MMIPB_SYNC_START, 0);
	
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Get_Token_Callback");
}

/****************************************************************************
**  Description : http disconected and logout
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/27/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_Login(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Login");

	/* Set http connect state */
	MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_CONNECTED);

	/* Get token */
#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER ) && (_MSC_VER >= 1400)))
#ifdef MMIPB_SYNC_USE_PC_TOKEN
    MMIAPIPC_get_token(TRUE, MMIPB_SYNC_Get_Token_Callback);
#else
    MMIAPIMSA_get_token(TRUE ,MSA_APP_TYPE_CONTACT_SYNC,  MMIPB_SYNC_Get_Token_Callback);
#endif
#endif	

	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Login");
}

/****************************************************************************
**  Description : http disconected and logout
** Global Resource Dependency:                                                  *
** Author: Shiwei.zhang                                                     *
** Note: 07/27/2015  
*****************************************************************************/
PUBLIC void MMIPB_SYNC_Logout(void)
{
	SCI_TRACE_LOW("+[MMIPBSYNC] MMIPB_SYNC_Logout, http_state = %d", MMIPB_SYNC_GetHttpState());

	MMIPB_CloseSyncCheckWaitDialog();
	
	//MMISRV_PBSYNC_CloseWaitingWindow();

	//MMIPB_DisplaybannerInfo(MMK_GetFocusWinId(), TXT_PB_SYNCPB_SYNC_DONE, PNULL);

	MMIPB_SYNC_SetHttpState(MMIPB_SYBC_CONNECT_DISCONNECTED);
	MMIPB_UpdatePBSyncSettingMenu();
	SCI_TRACE_LOW("-[MMIPBSYNC] MMIPB_SYNC_Logout");
}

#if 0
PUBLIC void MMIPB_SYNC_StartHeartBeat(void)
{
    if(0 != sync_heart_beat_timer_id)
    {
        MMK_StopTimer(sync_heart_beat_timer_id);
        sync_heart_beat_timer_id = 0;
    }
    
    sync_heart_beat_timer_id = MMK_CreateTimerCallback(MMIPB_SYNC_HEART_BEAT_TIMEOUT, 
		MMIPB_SYNC_Start, 
		PNULL, 
		TRUE);
}

PUBLIC void MMIPB_SYNC_StopHeartBeat(void)
{
    if(0 != sync_heart_beat_timer_id)
    {
        MMK_StopTimer(sync_heart_beat_timer_id);
        sync_heart_beat_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : testMMIPB_SYNC_GetListNodePosByO365Id
//  Global resource dependence : 
//  Author: shiwei.zhang
//  Note: 2015-7-8
/*****************************************************************************/
LOCAL uint32 Test_MMIPB_SYNC_GetListNodePosByO365Id()
{
    uint32 pos = 0xFFFF;
    MMIPB_CONTACT_T contact = {0};
    BOOLEAN found = FALSE;
    uint16 phone_contact_id = 0;
    int32 cmp_result = 0;
    char str_o365id[MMIPB_SYNC_O365_ID_LEN] = "2015-07-09T13:26:53Z";
    //char str_o365modiTime[MMIPB_SYNC_MODIFIEDTIME_LEN] = "2015-07-08T17:35:32Z";
    uint32 cmpmodiTime = 0x11111111;
    SCI_MEMCPY(contact.o365_id, str_o365id, sizeof(contact.o365_id));
    //SCI_MEMCPY(contact.o365_modifiedTime, str_o365modiTime, MMIPB_SYNC_MODIFIEDTIME_LEN);
    found = MMIPB_SYNC_GetListNodePosByO365Contact(&contact, &pos);
    if (found)
    {   		
        MMIPB_SYNC_ReadPhoneContactId(pos, &phone_contact_id);
        cmp_result = MMIPB_SYNC_CmpO365ModifiedTime(pos, cmpmodiTime);
        MMIPB_SYNC_SetPhoneContactChecked(pos, TRUE);
    }

    SCI_TRACE_LOW("pos = %d ",pos);
    SCI_TRACE_LOW("phone_contact_id = %d ",phone_contact_id);

    return pos;
}
#endif
#endif

#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC char pinyinFirstLetter(unsigned short hanzi)
{	
	int index = hanzi - HANZI_START;	
	if (index >= 0 && index <= HANZI_COUNT)	{		
		return firstLetterArray[index];	
	}	
	else	
	{		
		return '#';	
	}
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_WstrcmpWithPinyin(
								   const wchar * string1,
								   const wchar * string2
								   )
{
	int ret = 0 ;
	if( string1 == PNULL || string2 == PNULL )
	{
		if( string2 != PNULL )
		{
			return -1;
		}
		else if( string1 != PNULL )
		{
			return 1;
		}
		return 0;
	}
	else
	{
		char pinyin = '#';
		wchar wchar1 = *(wchar *)string1;
		wchar wchar2 = *(wchar *)string2;
		pinyin = pinyinFirstLetter(wchar1);
		if('#' != pinyin)
		{
			wchar1 = pinyin;
		}
		pinyin = pinyinFirstLetter(wchar2);
		if('#' != pinyin)
		{
			wchar2 = pinyin;
		}
		ret = wchar1 - wchar2;
		while( (!ret) && *string2 && *string1)/*lint !e613 */
		{
			++string1;/*lint !e613 */
			++string2;
			wchar1 = *(wchar *)string1;
			wchar2 = *(wchar *)string2;
			
			pinyin = pinyinFirstLetter(wchar1);
			if('#' != pinyin)
			{
				wchar1 = pinyin;
			}
			pinyin = pinyinFirstLetter(wchar2);
			if('#' != pinyin)
			{
				wchar2 = pinyin;
			}
			ret = wchar1 - wchar2;
		}
		/*lint -restore */
		return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) ); 
	}
}
#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

#ifdef MMI_PB_MULTILANG_SORT_SUPPORT
#include "mmipb_multilang_sort_array.c"

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIPB_MultiLang_SortBinarySearch(MMIPB_MULTILANG_SORT_T* sort_array, int sort_array_len, uint16 value)
{
	int low = 0;
	int high = sort_array_len - 1;
	while(low <= high)
	{
		int middle = (low + high)/2;
		if(sort_array[middle].unicode == value)
			return middle;
		//在左半边
		else if(sort_array[middle].unicode > value)
			high = middle - 1;
		//在右半边
		else
			low = middle + 1;
	}
	//没找到
	return -1;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIPB_MultiLang_FirstCharBinarySearch(MMIPB_MULTILANG_FIRST_CHAR_T* first_char_array, int first_char_array_len, uint16 value)
{
	int low = 0;
	int high = first_char_array_len - 1;
	while(low <= high)
	{
		int middle = (low + high)/2;
		if(first_char_array[middle].unicode == value)
			return middle;
		//在左半边
		else if(first_char_array[middle].unicode > value)
			high = middle - 1;
		//在右半边
		else
			low = middle + 1;
	}
	//没找到
	return -1;
}

MMISET_LANGUAGE_TYPE_E g_mmipb_multilang_sort_lang = MMISET_LANGUAGE_ENGLISH;

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_MultiLang_SetSortLang(MMISET_LANGUAGE_TYPE_E sort_lang)
{
    if (MMISET_LANGUAGE_TEST == sort_lang) // if Test language, use Englisht to sort
    {
        g_mmipb_multilang_sort_lang = MMISET_LANGUAGE_ENGLISH;
    }
    else
    {
        g_mmipb_multilang_sort_lang = sort_lang;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISET_LANGUAGE_TYPE_E MMIAPIPB_MultiLang_GetSortLang( void)
{
    return g_mmipb_multilang_sort_lang;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_MultiLang_GetUnicodeSortNum(uint16 text_unicode)
{
	uint16 result = MMIPB_MAX_SORT_NUM;
	int16 index = -1;
	//MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
	MMIPB_MULTILANG_SORT_ARRAY_T sort_array = {0};
	//MMIAPISET_GetLanguageType(&lang_type);
	sort_array.array_ptr = mmipb_multilang_sort_array[g_mmipb_multilang_sort_lang].array_ptr;
	sort_array.array_len= mmipb_multilang_sort_array[g_mmipb_multilang_sort_lang].array_len;
	//SCI_TRACE_LOW("GetUnicodeSortNum lang_type=%d",g_mmipb_multilang_sort_lang);

	if(PNULL == sort_array.array_ptr || 0 == sort_array.array_len)
	{
	}
	else
	{
		index = MMIPB_MultiLang_SortBinarySearch(sort_array.array_ptr, sort_array.array_len, text_unicode);
	}
	if( -1 != index)
	{
		result = (sort_array.array_ptr[index]).sort_num;
	}
	return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_MultiLang_GetFirstChar(uint16 text_unicode)
{
	uint16 result = MMIPB_MAX_SORT_NUM;
	int16 index = -1;
	//MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
	uint16 (*getFirstCharFunction)(uint16 wchar) = PNULL;
	//MMIAPISET_GetLanguageType(&lang_type);
	getFirstCharFunction = mmipb_multilang_getfirstchar_function_array[g_mmipb_multilang_sort_lang];
	//SCI_TRACE_LOW("GetFirstChar text_unicode=%d",text_unicode);

	if(getFirstCharFunction != PNULL)
	{
		uint16 firstChar = getFirstCharFunction(text_unicode);
		//SCI_TRACE_LOW("GetFirstChar firstChar=%d",firstChar);
		return firstChar;
	}
	else
	{
		return text_unicode;
	}
}

static const uint16 comm_alpha_order_len = sizeof(comm_alpha_order)/sizeof(MMIPB_MULTILANG_SORT_T);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_MultiLang_GetCommUnicodeSortNum(uint16 text_unicode)
{
	uint16 result = MMIPB_MAX_SORT_NUM;
	int16 index = -1;
	MMIPB_MULTILANG_SORT_ARRAY_T sort_array = {0};
	sort_array.array_ptr = comm_alpha_order;
	sort_array.array_len= comm_alpha_order_len;

	if(PNULL == sort_array.array_ptr || 0 == sort_array.array_len)
	{
	}
	else
	{
		index = MMIPB_MultiLang_SortBinarySearch(sort_array.array_ptr, sort_array.array_len, text_unicode);
	}
	if( -1 != index)
	{
		result = (sort_array.array_ptr[index]).sort_num;
	}
	return result;
}

#define MMIPB_SPACE_UNICODE 0x0020
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_CommUnicodeCmp(uint16 wchar1, uint16 wchar2)
{
	if(wchar1 == wchar2)
	{
		return 0;
	}
	else if(MMIPB_SPACE_UNICODE == wchar1)
	{
		return -1;
	}
	else if(MMIPB_SPACE_UNICODE == wchar2)
	{
		return 1;
	}	
	else
	{
		uint16 wchar1_sort_num = MMIPB_MultiLang_GetCommUnicodeSortNum(wchar1);
		uint16 wchar2_sort_num = MMIPB_MultiLang_GetCommUnicodeSortNum(wchar2);
		
		if(MMIPB_MAX_SORT_NUM == wchar1_sort_num && MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			int ret = wchar1 - wchar2;
			return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );  
		}
		else if(MMIPB_MAX_SORT_NUM == wchar1_sort_num || MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			if(MMIPB_MAX_SORT_NUM == wchar1_sort_num)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			int ret = wchar1_sort_num - wchar2_sort_num;
			return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_CommUnicodeCmp2(uint16 wchar1, uint16 wchar2)
{
	if(wchar1 == wchar2)
	{
		return 0;
	}
	else if(MMIPB_SPACE_UNICODE == wchar1)
	{
		return -1;
	}
	else if(MMIPB_SPACE_UNICODE == wchar2)
	{
		return 1;
	}	
	else
	{
		uint16 wchar1_sort_num = MMIPB_MultiLang_GetCommUnicodeSortNum(wchar1);
		uint16 wchar2_sort_num = MMIPB_MultiLang_GetCommUnicodeSortNum(wchar2);
		
		if(MMIPB_MAX_SORT_NUM == wchar1_sort_num && MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			int ret = wchar1 - wchar2;
			return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );  
		}
		else if(MMIPB_MAX_SORT_NUM == wchar1_sort_num || MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			if(MMIPB_MAX_SORT_NUM == wchar1_sort_num)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			int ret = wchar1_sort_num - wchar2_sort_num;
			if( 0 == ret)
			{
				int ret = wchar1 - wchar2;
				return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
			}
			else
			{
				return ( ret > 0 ? 1 : -1 );
			}
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_UnicodeCmp(uint16 wchar1, uint16 wchar2)
{
	if(wchar1 == wchar2)
	{
		return 0;
	}
	else if(MMIPB_SPACE_UNICODE == wchar1)
	{
		return -1;
	}
	else if(MMIPB_SPACE_UNICODE == wchar2)
	{
		return 1;
	}	
	else
	{
		uint16 wchar1_sort_num = MMIPB_MultiLang_GetUnicodeSortNum(wchar1);
		uint16 wchar2_sort_num = MMIPB_MultiLang_GetUnicodeSortNum(wchar2);
		
		if(MMIPB_MAX_SORT_NUM == wchar1_sort_num && MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			//return MMIPB_MultiLang_CommUnicodeCmp(wchar1,wchar2);
			int ret = wchar1 - wchar2;
			return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
		}
		else if(MMIPB_MAX_SORT_NUM == wchar1_sort_num || MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			if(MMIPB_MAX_SORT_NUM == wchar1_sort_num)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			int ret = wchar1_sort_num - wchar2_sort_num;
			return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_UnicodeCmp2(uint16 wchar1, uint16 wchar2)
{
	if (wchar1 == wchar2)
	{
		return 0;
	}
	else if (MMIPB_SPACE_UNICODE == wchar1)
	{
		return -1;
	}
	else if(MMIPB_SPACE_UNICODE == wchar2)
	{
		return 1;
	}	
	else
	{
		uint16 wchar1_sort_num = MMIPB_MultiLang_GetUnicodeSortNum(wchar1);
		uint16 wchar2_sort_num = MMIPB_MultiLang_GetUnicodeSortNum(wchar2);
		
		if (MMIPB_MAX_SORT_NUM == wchar1_sort_num && MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{			
			//return MMIPB_MultiLang_CommUnicodeCmp2(wchar1,wchar2);
			int ret = wchar1 - wchar2;
			return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
		}
		else if(MMIPB_MAX_SORT_NUM == wchar1_sort_num || MMIPB_MAX_SORT_NUM == wchar2_sort_num)
		{
			if(MMIPB_MAX_SORT_NUM == wchar1_sort_num)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			int ret = wchar1_sort_num - wchar2_sort_num;
			if( 0 == ret)
			{
				int ret = wchar1 - wchar2;
				return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) );
			}
			else
			{
				return ( ret > 0 ? 1 : -1 );
			}
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_Wstrcmp
(
 const wchar * string1,
 const wchar * string2
 )
{
	int ret = 0 ;
	if( string1 == PNULL || string2 == PNULL )
	{
		if( string2 != PNULL )
		{
			return -1;
		}
		else if( string1 != PNULL )
		{
			return 1;
		}
		return 0;
	}
	else if( *string1 == 0 && *string2 == 0 )
	{
		return 0;
	}	
	else if( *string1 == 0 || *string2 == 0 )
	{
		if( *string1 == 0 )
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}	
	else
	{
		const wchar * string1_old=string1;
		const wchar * string2_old=string2;
		//第一次比较，相同排序不比较unicode大小
		ret = MMIPB_MultiLang_UnicodeCmp(*(wchar *)string1, *(wchar *)string2);
		while( (!ret) && *string2 && *string1)
		{
			++string1;
			++string2;
			ret = MMIPB_MultiLang_UnicodeCmp(*(wchar *)string1, *(wchar *)string2);
		}
		if(0 == ret)
		{
			//完全相等，进行第二次比较，相同排序比较unicode大小
			if(0 == *string1 && 0 == *string2)
			{
				ret = MMIPB_MultiLang_UnicodeCmp2(*(wchar *)string1_old, *(wchar *)string2_old);
				while( (!ret) && *string1_old && *string1_old)
				{
					++string1_old;
					++string1_old;
					ret = MMIPB_MultiLang_UnicodeCmp2(*(wchar *)string1_old, *(wchar *)string2_old);
				}
			}
			else
			{
				if(0 == *string1)
				{
					if(0 != *string2)
					{
						ret = -1;
					}				
				}
				else
				{
					if(0 == *string2)
					{
						ret = 1;
					}
				}
			}
		}
	}
	return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) ); 
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC wchar MMIPB_MultiLang_GetBaseChar
(
 wchar string1
 )
{
	wchar wchar1 = string1;
	//输入A，可查á
	wchar1 = MMIPB_MultiLang_comm_GetFirstChar(string1);
	//简体中文语言，输入A，可查啊
	if(MMISET_LANGUAGE_SIMP_CHINESE == g_mmipb_multilang_sort_lang
		&& wchar1 == string1
		)
	{
		wchar1 = MMIPB_MultiLang_zh00_GetFirstChar(string1);
	}
	return wchar1;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_String1BaseCharCmp
(
 wchar string1,
 wchar string2
 )
{
	int result = 0;
	if((string2 >= 0x0041 && string2 <= 0x005a) || (string2 >= 0x0061 && string2 <= 0x007a))
	{
		wchar wchar1[2] = {0} ;
		wchar wchar2[2] = {0};
		wchar1[0] = MMIPB_MultiLang_GetBaseChar(string1);
		wchar2[0] = string2;
		MMIAPICOM_Wstrupper(wchar1);
		MMIAPICOM_Wstrupper(wchar2);
		result = wchar1[0] - wchar2[0];
	}
	else
	{
		result = string1 - string2;
	}
	return ( result > 0 ? 1 : ( result < 0 ? -1 : 0 ) );    
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_Wstrncmp(
                              const wchar* string1,
                              const wchar* string2,
                              size_t count
                              )
{
	int result = 0;
	if (!count)
	{
		return(0);
	}
	
	if( string1 == PNULL || string2 == PNULL )
	{
		if( string1 == PNULL )
		{
			return -1;
		}
		else 
		{
			return 1;
		}
	}
	else
	{
		while ( --count && *string1 && !MMIPB_MultiLang_String1BaseCharCmp(*string1, *string2))
		{
			string1++;
			string2++;
		}
		result = MMIPB_MultiLang_String1BaseCharCmp(*string1, *string2);
		return ( result > 0 ? 1 : ( result < 0 ? -1 : 0 ) );    
	}
}

/*****************************************************************************/
//  Description : check whether string1 contain string2
//  Global resource dependence : 
//  Author:
//  RETRUN: 
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_MultiLang_Wstrnstr(
                                  const wchar  *str1_ptr,
                                  uint16       str1_len,
                                  const wchar  *str2_ptr,
                                  uint16       str2_len
                                  )
{
	BOOLEAN is_match = FALSE;
	int32   i = 0;
	
	if( str2_len <= str1_len)
	{
		for(i = 0; i < str1_len-str2_len+1; i++, str1_ptr++)
		{
			if(!MMIPB_MultiLang_String1BaseCharCmp(*str1_ptr,*str2_ptr) && MMIPB_MultiLang_Wstrncmp(str1_ptr, str2_ptr, str2_len) == 0)
			{
				is_match = TRUE;
				break;
			}
		}
	}
	return is_match;
}

/*****************************************************************************/
//  Description : check whether string1 contain string2 ext
//  Global resource dependence : 
//  Author:
//  RETRUN: 
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_MultiLang_WstrnstrExt(
                                  const wchar  *str1_ptr,
                                  uint16       str1_len,
                                  const wchar  *str2_ptr,
                                  uint16       str2_len,
                                  BOOLEAN        str_flag     //是否字符串区分大小写
                                  )
{
    BOOLEAN result = FALSE;
    wchar   *str1 = PNULL;
    wchar   *str2 = PNULL;
    
    if (!str_flag)
    {
        str1 =  SCI_ALLOCA(sizeof(wchar)*(str1_len+1));
        SCI_MEMSET(str1,0x00,(sizeof(wchar)*(str1_len+1)));
        MMIAPICOM_Wstrncpy(str1,str1_ptr,str1_len);
        
        str2 =  SCI_ALLOCA(sizeof(wchar)*(str2_len+1));
        SCI_MEMSET(str2,0x00,(sizeof(wchar)*(str2_len+1)));
        MMIAPICOM_Wstrncpy(str2,str2_ptr,str2_len);
        
        MMIAPICOM_Wstrupper(str1);
        MMIAPICOM_Wstrupper(str2);
        
        result = MMIPB_MultiLang_Wstrnstr(str1,str1_len,str2,str2_len);
    }
    else
    {
        result = MMIPB_MultiLang_Wstrnstr(str1_ptr,str1_len,str2_ptr,str2_len);
    }

    if (!str_flag)
    {
        SCI_FREE(str1); 
        SCI_FREE(str2);
    }

    return result;
}
#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int ContactListCmp(uint32 base_index, void *compare_data, void *list)
{
    MMIPB_CONTACT_LIST_NODE_T  *contact_list = (MMIPB_CONTACT_LIST_NODE_T * )list;
    //MMIPB_NAME_T         name     = {0};
    MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
    MMIPB_NAME_LETTERS_T  *input_str = (MMIPB_NAME_LETTERS_T *)compare_data;
    MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
    int                     ret = 0;
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    wchar*                   input_full_letter_ptr = PNULL;
    wchar*                   cmp_full_letter_ptr =PNULL;
    uint16                  pos_index = 0;
#endif
    //uint16 name_len = 0;
    //uint16 i = 0;
    if(PNULL == contact_list)
    {
        //SCI_TRACE_LOW:"ContactListCmp valid parameter !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1255_112_2_18_2_43_21_179,(uint8*)"");
        return -1;
    }
    letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_LETTERS_T));
    if(PNULL == letters_ptr)
    {
        return -1;
    }
    
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    input_full_letter_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_NAME_MAX_LEN*6+1));
    if(PNULL == input_full_letter_ptr)
    {
        SCI_FREE(letters_ptr);
        return -1;
    }
    cmp_full_letter_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_NAME_MAX_LEN*6+1));
    if(PNULL == cmp_full_letter_ptr)
    {
        SCI_FREE(letters_ptr);
        SCI_FREE(input_full_letter_ptr);
    }
#endif    
    MMIPB_GetContactNameNumberByIndex(&cmp_str, base_index);

#ifdef MMI_PB_SURNAME_SUPPORT
    MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_FULL_CHAR/*MMIPB_NAME_FULL_CHAR_SPACER*/, TRUE,letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
    MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR_SPACER, TRUE,letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/

#ifdef MMIPB_COVERT_LETTER_SUPPORT
    for(pos_index=0;pos_index<input_str->full_letters_len; pos_index++)
    {
        input_full_letter_ptr[pos_index] = ConvertLetters(input_str->full_letters[pos_index]);
    }
    for(pos_index=0;pos_index<letters_ptr->full_letters_len; pos_index++)
    {
        cmp_full_letter_ptr[pos_index] = ConvertLetters(letters_ptr->full_letters[pos_index]);  
    }

    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
    ret = MMIAPICOM_Wstrcmp(cmp_full_letter_ptr, input_full_letter_ptr);
    #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    ret = MMIPB_MultiLang_Wstrcmp(cmp_full_letter_ptr, input_full_letter_ptr);
    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    if(ret == 0)
    {
       #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
       ret = MMIAPICOM_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
       #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
       ret = MMIPB_MultiLang_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
       #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    }
#else
    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
    ret = MMIAPICOM_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
    #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    ret = MMIPB_MultiLang_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
#endif

#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
    if(ret < 0 
      && !MMIAPIIM_IsPunct(*letters_ptr->full_letters) 
      &&MMIAPIIM_IsPunct(*input_str->full_letters) 
      )
    {
        //符号不能排在非符号的后面
        ret = 1;
    }
    else if(ret > 0 
      && MMIAPIIM_IsPunct(*letters_ptr->full_letters) 
      &&!MMIAPIIM_IsPunct(*input_str->full_letters) 
      )
    {
        //符号不能排在非符号的后面
        ret = -1;
    }	
    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

    //SCI_TRACE_LOW("ContactListCmp ret %d ",ret);
    SCI_FREE(letters_ptr);
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    SCI_FREE(cmp_full_letter_ptr);
    SCI_FREE(input_full_letter_ptr);
#endif
    return ret;
}

/*****************************************************************************/
//  Description : 插入一个节点到记录表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactListNode(
                                                 MMIPB_CONTACT_T *contact_ptr//[IN]
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    uint16                count = MMIPB_INVALID_INDEX;
    //uint16                first_wchar = 0;
    uint32                pos = 0;
    uint32                i = 0;
    MMIPB_NAME_LETTERS_T  *letters_ptr = PNULL;
    MMIPB_CONTACT_BASE_INFO_T *simple_info_ptr = PNULL;

    wchar *key_wstr_ptr = PNULL;
    
    if(contact_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    if(s_pb_contact_list.list_info.count >= s_pb_contact_list.max_count)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_InsertContactListNode max_count %d, count %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1296_112_2_18_2_43_21_180,(uint8*)"dd", s_pb_contact_list.max_count, s_pb_contact_list.list_info.count);
        return result;
    }
    letters_ptr = (MMIPB_NAME_LETTERS_T  *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
    if(PNULL == letters_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_InsertQListNode letters_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1302_112_2_18_2_43_21_181,(uint8*)"");
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
    //#endif
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
#ifdef MMI_PB_SURNAME_SUPPORT
    MMIPB_GetNameLettersWithSurname(&contact_ptr->name, &contact_ptr->surname, MMIPB_NAME_FULL_CHAR/*MMIPB_NAME_FULL_CHAR_SPACER*/|MMIPB_NAME_HEAD_CHAR,TRUE,letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
    MMIPB_GetNameLetters(&contact_ptr->name, MMIPB_NAME_FULL_CHAR_SPACER|MMIPB_NAME_HEAD_CHAR,TRUE,letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
    search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
    search_info.compare_base_data = letters_ptr;
    search_info.start_pos = 0;
    //如果是手机联系人保存到先保存手机联系人缓存表
    if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
    {
        {

            count = s_pb_phone_contact_list.count_count;
            if(count < s_pb_phone_contact_list.max_count 
               && s_pb_phone_contact_list.contact_table != PNULL
              )
            {
                //添加在最后
                //插入该节点
                SCI_MEMSET(&s_pb_phone_contact_list.contact_table[count], 0x00, sizeof(MMIPB_PHONE_CONTACT_NODE_T));
                SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].num_type, 
                    contact_ptr->num_type, 
                    sizeof(s_pb_phone_contact_list.contact_table[count].num_type));
                //number
                SCI_MEMCPY(&s_pb_phone_contact_list.contact_table[count].number, &contact_ptr->number, sizeof(contact_ptr->number));
                //name
                if(contact_ptr->name.wstr_len == 0)
                {
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
                }
                else 
                {
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_len = contact_ptr->name.wstr_len;
                    s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->name.wstr_len,s_pb_mempool_handle);
                    if(s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr != PNULL)
                    {
                        SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].name_str.wstr_ptr, contact_ptr->name.wstr, sizeof(wchar)*contact_ptr->name.wstr_len);
                    }
                }

#ifdef MMI_PB_SURNAME_SUPPORT
                //surname
                if(contact_ptr->surname.wstr_len == 0)
                {
                    s_pb_phone_contact_list.contact_table[count].surname_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
                }
                else 
                {
                    s_pb_phone_contact_list.contact_table[count].surname_str.wstr_len = contact_ptr->surname.wstr_len;
                    s_pb_phone_contact_list.contact_table[count].surname_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->surname.wstr_len,s_pb_mempool_handle);
                    if(s_pb_phone_contact_list.contact_table[count].surname_str.wstr_ptr != PNULL)
                    {
                        SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].surname_str.wstr_ptr, contact_ptr->surname.wstr, sizeof(wchar)*contact_ptr->surname.wstr_len);
                    }
                }
#endif /*MMI_PB_SURNAME_SUPPORT*/

            //email
#ifdef MMIPB_MAIL_SUPPORT
                if(contact_ptr->mail.wstr_len == 0)
                {
                    s_pb_phone_contact_list.contact_table[count].mail_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
                }
                else 
                {
                    s_pb_phone_contact_list.contact_table[count].mail_str.wstr_len = contact_ptr->mail.wstr_len;
                    s_pb_phone_contact_list.contact_table[count].mail_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->mail.wstr_len,s_pb_mempool_handle);
                    if(s_pb_phone_contact_list.contact_table[count].mail_str.wstr_ptr != PNULL)
                    {
                        SCI_MEMCPY(s_pb_phone_contact_list.contact_table[count].mail_str.wstr_ptr, 
                                  contact_ptr->mail.wstr, 
                                  sizeof(wchar)*contact_ptr->mail.wstr_len);
                    }
                }
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                s_pb_phone_contact_list.contact_table[count].preferred_sim = contact_ptr->preferred_sim;
#endif /*MMI_MULTI_SIM_SYS_SINGLE*/
                s_pb_phone_contact_list.count_count++;
            }
        }
    }
#ifdef SNS_SUPPORT
    //add sns base information
    else if(MMIPB_IsSNSContact(contact_ptr->storage_id))
    {
        count = s_pb_sns_contact_list.current_count;
        if(count < s_pb_sns_contact_list.max_count 
           && s_pb_sns_contact_list.contact_table != PNULL
          )
        {
            //添加在最后
            //插入该节点
            s_pb_sns_contact_list.contact_table[count].group = contact_ptr->union_t.group;
            //name
            if(contact_ptr->name.wstr_len == 0)
            {
                s_pb_sns_contact_list.contact_table[count].name_str.wstr_len = MMIPB_EMPTY_STRING_FLAG;
            }
            else 
            {
                s_pb_sns_contact_list.contact_table[count].name_str.wstr_len = contact_ptr->name.wstr_len;
                s_pb_sns_contact_list.contact_table[count].name_str.wstr_ptr = SCI_MPALLOC(sizeof(wchar)*contact_ptr->name.wstr_len, s_pb_mempool_handle);
                if(s_pb_sns_contact_list.contact_table[count].name_str.wstr_ptr != PNULL)
                {
                    SCI_MEMCPY(s_pb_sns_contact_list.contact_table[count].name_str.wstr_ptr, contact_ptr->name.wstr, sizeof(wchar)*contact_ptr->name.wstr_len);
                }
            }
            s_pb_sns_contact_list.current_count++;
                
        }
    }
#endif
    //find insert postion in s_pb_contact_list
    if(s_pb_contact_list.list_info.count > 0 && s_pb_contact_list.list_info.count < s_pb_contact_list.max_count)
    {
        search_info.end_pos = s_pb_contact_list.list_info.count -1;
        MMIAPICOM_BinSearch(&search_info,
            ContactListCmp,
            &pos,
            s_pb_contact_list.list);
    }
    //insert node 
    if(s_pb_contact_list.list_info.count < s_pb_contact_list.max_count && pos < s_pb_contact_list.max_count)
    {
        //SCI_DisableIRQ();
        // 顺序后移
        for(i=s_pb_contact_list.list_info.count; i>pos; i--)
        {
            s_pb_contact_list.list[i] = s_pb_contact_list.list[i-1];
        }   
        //clear old data
        SCI_MEMSET(&s_pb_contact_list.list[pos], 0x00, sizeof(MMIPB_CONTACT_LIST_NODE_T));
        //group
        s_pb_contact_list.list[pos].group = contact_ptr->union_t.group;
        //contact id
        s_pb_contact_list.list[pos].contact_id = contact_ptr->contact_id;

#ifdef MMIPB_SYNC_WITH_O365		
        SCI_MEMCPY(s_pb_contact_list.list[pos].o365_id, contact_ptr->o365_id, sizeof(s_pb_contact_list.list[pos].o365_id));
        s_pb_contact_list.list[pos].o365_modifiedTime = contact_ptr->o365_modifiedTime;
        s_pb_contact_list.list[pos].phone_modifiedTime = contact_ptr->phone_modifiedTime;
        s_pb_contact_list.list[pos].checked = FALSE;
#endif
        //set index 
        s_pb_contact_list.list[pos].index = count;
        //set start char
        #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
        if(MMIAPIIM_IsPunct(letters_ptr->full_letters[0]))
        {
            s_pb_contact_list.list[pos].first_char = '#';  
        }
        else
        #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
        {
            #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                s_pb_contact_list.list[pos].first_char = letters_ptr->full_letters[0];
            #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                s_pb_contact_list.list[pos].first_char = MMIPB_MultiLang_GetFirstChar(letters_ptr->full_letters[0]);
            #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
        }
        if(pos == 0 || s_pb_contact_list.list[pos].first_char != s_pb_contact_list.list[pos -1].first_char)
        {
            //第一个记录，或者和上一个记录的首字母不相同
            s_pb_contact_list.list_info.first_char_count++;
        }
        //storage id
        s_pb_contact_list.list[pos].short_storage_id = StorageIDToShortID(contact_ptr->storage_id);
#ifdef MMIPB_UID_SUPPORT        
        s_pb_contact_list.list[pos].uid = contact_ptr->uid;
#endif
#ifdef CMCC_VCARD_SUPPORT
        if(MMIPB_IsUsim(contact_ptr->storage_id))
        {
            s_pb_contact_list.list[pos].is_read_ext = FALSE;
        }
        else
        {
            s_pb_contact_list.list[pos].is_read_ext = TRUE;
        }
        //SCI_TRACE_LOW("[baokun] MMIPB_InsertContactListNode is_read_ext %d",s_pb_contact_list.list[pos].is_read_ext);
#endif
        s_pb_contact_list.list[pos].photo_id_flag = contact_ptr->photo_id_flag;
        s_pb_contact_list.list_info.count++;
        simple_info_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_BASE_INFO_T));
        if(PNULL != simple_info_ptr)
        {
            SCI_MEMSET(simple_info_ptr, 0x00, sizeof(MMIPB_CONTACT_BASE_INFO_T));
            MMIPB_ReadContactSimpleInfo(pos, simple_info_ptr);
            //adjust base info
            result = AdjustContactListInfo(simple_info_ptr,
                TRUE,
                &s_pb_contact_list.list_info);
            SCI_FREE(simple_info_ptr);            
        }                
        //update contact id usage condition
        if(MMIPB_IsPhoneContact(contact_ptr->storage_id))
        {
            //更新普通联系人的id使用情况表
            SetEntryIdFlag(TRUE, contact_ptr->contact_id, &s_pb_phone_contact_id_table);
        }
        if(letters_ptr->head_letters_len > 0)
        {
            #ifndef MMI_PB_SURNAME_SUPPORT
            key_wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN +1)*sizeof(wchar));
            #else
            key_wstr_ptr = SCI_ALLOCA(((MMIPB_NAME_MAX_LEN +1)*2)*sizeof(wchar));
            #endif
            if(key_wstr_ptr != PNULL)
            {
                SCI_MEMSET(key_wstr_ptr, 0x00, (MMIPB_NAME_MAX_LEN +1)*sizeof(wchar));
                MMIIM_GetWcharKeyValueByTable(letters_ptr->head_letters, key_wstr_ptr, FALSE);
            }
        }
        MMIPB_InsertQListNode(pos, 
            letters_ptr->head_letters, 
            letters_ptr->head_letters_len,
            key_wstr_ptr,
            letters_ptr->head_letters_len);
        if(key_wstr_ptr != PNULL)
        {
            SCI_FREE(key_wstr_ptr);
        }
        SCI_FREE(letters_ptr);
    }
    SCI_PutSemaphore(s_contact_list_semphore_p);
  
    return result;
}
/*****************************************************************************/
//  Description : 从记录表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactListNode(
                                                 uint16 contact_id,
                                                 uint16 storage_id
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 i = 0;
    //uint32 dest_index = 0;
    //uint32 del_count = 0;
    uint16 index = 0;
    uint16 cache_index = 0;
    //uint16 storage_id = 0;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPB_CONTACT_BASE_INFO_T simple_info = {0};
    index = MMIPB_GetContactListIndex(contact_id, storage_id);
   
    
    if(index < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL)
    {
        result = MMIPB_ERROR_SUCCESS;
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); 
        MMIPB_ReadContactSimpleInfo(index, &simple_info);
        cache_index = s_pb_contact_list.list[index].index;
        //从缓存表中删除

        //10手机或USIM
        if(MMIPB_IsPhoneContact(storage_id))
        {

            if(cache_index < s_pb_phone_contact_list.count_count 
                && PNULL != s_pb_phone_contact_list.contact_table
                )
            {
                //删除从手机缓存表中删除该节点
                //delete name str 
                if(s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_ptr);
                    s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_ptr = PNULL;
                    s_pb_phone_contact_list.contact_table[cache_index].name_str.wstr_len = 0;
                }
                #ifdef MMI_PB_SURNAME_SUPPORT
                //surname
                if(s_pb_phone_contact_list.contact_table[cache_index].surname_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_phone_contact_list.contact_table[cache_index].surname_str.wstr_ptr);
                    s_pb_phone_contact_list.contact_table[cache_index].surname_str.wstr_ptr = PNULL;
                    s_pb_phone_contact_list.contact_table[cache_index].surname_str.wstr_len = 0;
                }
                #endif /*MMI_PB_SURNAME_SUPPORT*/

                //delete mail str
                if(s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_ptr);
                    s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_ptr = PNULL;
                    s_pb_phone_contact_list.contact_table[cache_index].mail_str.wstr_len = 0;
                }
                for(i = cache_index; i < s_pb_phone_contact_list.count_count -1; i++)
                {
                    s_pb_phone_contact_list.contact_table[i] = s_pb_phone_contact_list.contact_table[i + 1];
                }
                s_pb_phone_contact_list.count_count--;
            }
        }
#ifdef SNS_SUPPORT
        //delete from sns cache
        else if(MMIPB_IsSNSContact(storage_id))
        {
            if(cache_index < s_pb_sns_contact_list.current_count 
                && PNULL != s_pb_sns_contact_list.contact_table
                )
            {
                //删除从sns缓存表中删除该节点
                //delete name str 
                if(s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_ptr != PNULL)
                {
                    SCI_MPFREE(s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_ptr);
                    s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_ptr = PNULL;
                    s_pb_sns_contact_list.contact_table[cache_index].name_str.wstr_len = 0;
                }
                for(i = cache_index; i < s_pb_sns_contact_list.current_count -1; i++)
                {
                    s_pb_sns_contact_list.contact_table[i] = s_pb_sns_contact_list.contact_table[i + 1];
                }
                s_pb_sns_contact_list.current_count--;
            }            
        }
#endif

#ifdef CMCC_VCARD_SUPPORT
          else if(MMIPB_IsUsim(storage_id))
          {
              MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
              MMIPB_BCD_NUMBER_LIST_T *free_list = PNULL;
              if(s_pb_contact_list.list[index].num_mail_list != PNULL)
              {
                  number_list = s_pb_contact_list.list[index].num_mail_list->number_list;
                  while(number_list != PNULL)
                  {
                      free_list = number_list;
                      number_list = number_list->next_ptr;
                      SCI_MPFREE(free_list);
                      free_list = PNULL;
                  }
                  if(s_pb_contact_list.list[index].num_mail_list->mail_str.wstr_ptr != PNULL)
                  {
                      SCI_MPFREE(s_pb_contact_list.list[index].num_mail_list->mail_str.wstr_ptr);
                  }
                  SCI_MPFREE(s_pb_contact_list.list[index].num_mail_list);
                  s_pb_contact_list.list[index].num_mail_list = PNULL;

              }
          }
#endif 
        //从索引表中
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(i >= index && i < s_pb_contact_list.list_info.count -1)
            {
               s_pb_contact_list.list[i] = s_pb_contact_list.list[i + 1];
            }


            if(
               storage_id == ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)
               && s_pb_contact_list.list[i].index > cache_index
              )

            {
                s_pb_contact_list.list[i].index--;
            }

        }
        
        //该节点将被删除，调整基本信息
        AdjustContactListInfo(&simple_info, FALSE, &s_pb_contact_list.list_info);            
        s_pb_contact_list.list_info.count--;
        
        //update contact id usage condition
        if(MMIPB_IsPhoneContact(storage_id))
        {
            //更新普通联系人的id使用情况表
            SetEntryIdFlag(FALSE, contact_id, &s_pb_phone_contact_id_table);
        }
        SCI_PutSemaphore(s_contact_list_semphore_p); 
        //从快速查找表中删除
        //delete index from qsort list
        MMIPB_DeleteQListNode(index);
    }
    return result;
}

/*****************************************************************************/
//  Description : Delete one group's contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOneGroupContactList(uint32 group)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 i = 0;
    uint16 del_count = 0;
    MMIPB_CONTACT_BASE_INFO_T contact_info = {0};

    if(group > 0)
    {
        //delete qsort list
        for(i = 0; i < s_pb_qsort_list.count; i++)
        {
            if(group & s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].group)
            {
                //该节点需要删除
                del_count++;                
            }
            else
            {
                if(del_count > 0)
                {
                    s_pb_qsort_list.list[i - del_count] = s_pb_qsort_list.list[i];
                }
            }
        }
        if(del_count <= s_pb_qsort_list.count)
        {
            s_pb_qsort_list.count = s_pb_qsort_list.count - del_count;
        }

        //delete contact list
        del_count = 0;
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(group & s_pb_contact_list.list[i].group)
            {
                //该节点需要删除
                if(MMIPB_ERROR_SUCCESS == MMIPB_ReadContactSimpleInfo(i, &contact_info))
                {
                    AdjustContactListInfo(&contact_info, FALSE, &s_pb_contact_list.list_info);                        
                    SCI_MEMSET(&contact_info, 0x00, sizeof(contact_info));
                }
                del_count++;                
            }
            else
            {
                if(del_count > 0)
                {
                    s_pb_contact_list.list[i - del_count] = s_pb_contact_list.list[i];
                }
            }
        }
        if(del_count <= s_pb_contact_list.list_info.count)
        {
            s_pb_contact_list.list_info.count = s_pb_contact_list.list_info.count - del_count;
        }        
        result = MMIPB_ERROR_SUCCESS;
    }    
    return result;
}
#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr)
{
    uint16 i = 0;
    BOOLEAN ret = FALSE;
    if(PNULL == entry_id_ptr || PNULL == storage_id_ptr)
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_GetcontactIdByUid PNULL == entry_id_ptr || PNULL == storage_id_ptr");
        return ret;
    }
    if(s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(uid == s_pb_contact_list.list[i].uid)
            {
                *entry_id_ptr = s_pb_contact_list.list[i].contact_id;
                *storage_id_ptr = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                ret = TRUE;
                break;
            }
        }
        
    }
    return ret;
}



/*****************************************************************************/
//  Description : 根据记录uid 搜索对应记录的entry_id和storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid)
{
    uint16 i = 0;
    BOOLEAN ret = FALSE;
    MMIPB_CONTACT_T contact_info={0};
    if(PNULL == uid)
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_GetUidByContactId PNULL == uid");
        return ret;
    }
    if(MMIPB_ERROR_SUCCESS ==  MMIPB_GetContactInfo(entry_id, storage_id, &contact_info))
    {
        *uid = contact_info.uid;
        
    }
    return ret;
}
#endif
/*****************************************************************************/
//  Description : 根据记录id和storage id来确定该记录在索引表的索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactListIndex(
                                        uint16 contact_id,
                                        uint16 stoage_id
                                        )
{
    
    uint16 index = MMIPB_INVALID_INDEX;
    uint16 i = 0;
    
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER); ;
    if(contact_id > 0 && s_pb_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_contact_list.list_info.count; i++)
        {
            if(contact_id == s_pb_contact_list.list[i].contact_id
                && stoage_id == ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)
                )
            {
                index = i;
                break;
            }
        }
        
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    
    return index;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 根据记录id和storage id来确定该记录在常用联系人索引表的索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMostUsedContactListIndex(
                                                 uint16 contact_id,
                                                 uint16 stoage_id
                                                 )
{
    uint16 index = MMIPB_INVALID_INDEX;
    uint16 i = 0;
    
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
    if(contact_id > 0 && s_pb_mostused_contact_list.contact_table != PNULL)
    {
        for(i = 0; i < s_pb_mostused_contact_list.count; i++)
        {
            if(contact_id == s_pb_mostused_contact_list.contact_table[i].entry_id
                && stoage_id == s_pb_mostused_contact_list.contact_table[i].storage_id
                )
            {
                index = i;
                break;
            }
        }
        
    }
    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p); 
    
    return index;
}
/*****************************************************************************/
//  Description : 读常用联系人记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactCount(
                                                    uint16  *count_ptr//[OUT]
                                                    )
{
    if(count_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    *count_ptr = s_pb_mostused_contact_list.count;
    //SCI_RestoreIRQ();
    
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 读常用联系人索引表的节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContactListNode(
                                                       uint16 index, 
                                                       MMIPB_MOSTUSED_CONTACT_NODE_T *data_ptr//[OUT]
                                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
    if(index < s_pb_mostused_contact_list.count)
    {
        *data_ptr = s_pb_mostused_contact_list.contact_table[index];
        result = MMIPB_ERROR_SUCCESS;
    }
    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p); 
    return result;
}

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int MostUsedContactListCmp(uint32 base_index, void *compare_data, void *list)
{
    MMIPB_MOSTUSED_CONTACT_NODE_T   *contact_list = (MMIPB_MOSTUSED_CONTACT_NODE_T * )list;
    
    uint32       used_freq = (uint32)compare_data;
    int ret = 0;
    
    
    if(PNULL == contact_list)
    {
        //SCI_TRACE_LOW:"MostUsedContactListCmp valid parameter !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1764_112_2_18_2_43_22_182,(uint8*)"");
        return -1;
    }
    
    ret = (int32)used_freq - (int32)contact_list[base_index].used_freq;
    
    //SCI_TRACE_LOW:"MostUsedContactListCmp ret %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1770_112_2_18_2_43_22_183,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : 插入一个节点到常用联系人索引表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertMostUsedContactListNode(
                                                         MMIPB_CONTACT_T *contact_ptr//[IN]
                                                         )
{
    MMIPB_ERROR_E       result = MMIPB_ERROR_ERROR;
    uint32              insert_pos = 0;
    uint16              i = 0; 
    MMI_BIN_SEARCH_INFO_T search_info = {0};
    
    if((PNULL == contact_ptr) || (PNULL == s_pb_mostused_contact_list.contact_table))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_InsertMostUsedContactListNode invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1789_112_2_18_2_43_22_184,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    
    // 将号码加入到insert列表，同时完成在该列表中的排序
    
    if(s_pb_mostused_contact_list.count < MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS)
    {
        SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
        search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
        search_info.start_pos = 0;
        search_info.end_pos = s_pb_mostused_contact_list.count -1;
        search_info.compare_base_data = (void *)contact_ptr->union_t.used_freq;
        //查找插入位置
        MMIAPICOM_BinSearch(&search_info,
            MostUsedContactListCmp,
            &insert_pos,
            s_pb_mostused_contact_list.contact_table
            );
        // 顺序后移
        for (i=s_pb_mostused_contact_list.count; i>insert_pos; i--)
        {
            s_pb_mostused_contact_list.contact_table[i] = s_pb_mostused_contact_list.contact_table[i-1];
            //s_pb_mostused_contact_list.visibility_table[i] = s_pb_mostused_contact_list.visibility_table[i-1];
        }   
        SCI_MEMSET(&s_pb_mostused_contact_list.contact_table[insert_pos], 0x00, sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T));
        //插入
        s_pb_mostused_contact_list.contact_table[insert_pos].entry_id = contact_ptr->contact_id;
        s_pb_mostused_contact_list.contact_table[insert_pos].used_freq = contact_ptr->union_t.used_freq;  
        s_pb_mostused_contact_list.contact_table[insert_pos].storage_id = contact_ptr->storage_id;
#ifndef PB_SUPPORT_LOW_MEMORY
        SCI_MEMCPY(&s_pb_mostused_contact_list.contact_table[insert_pos].number, &contact_ptr->number[0], sizeof(MMIPB_BCD_NUMBER_T));
#endif
        //s_pb_mostused_contact_list.visibility_table[insert_pos] = 1;
        s_pb_mostused_contact_list.count++;
        
        result = MMIPB_ERROR_SUCCESS;
        //SCI_RestoreIRQ();
        //update contact id usage condition
        //更新常用联系人的id使用情况表
        SetEntryIdFlag(TRUE, contact_ptr->contact_id, &s_pb_mostused_contact_id_table);
        SCI_PutSemaphore(s_mostusedcontext_list_semphore_p); 
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 从常用联系人索引表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteMostUsedContactListNode(
                                                         uint16 contact_id,
                                                         uint16 storage_id
                                                         )
{
    uint16 i =0;
    uint32 prority = 0;
    uint16 contact_index = MMIPB_GetMostUsedContactListIndex(contact_id, storage_id);
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    
    if(contact_index < s_pb_mostused_contact_list.count && s_pb_mostused_contact_list.contact_table != PNULL)
    {
        SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER); 
        prority = (s_pb_mostused_contact_list.contact_table[contact_index].used_freq & 0xFFFF);         
        //顺序前移
        for (i=contact_index; i<(s_pb_mostused_contact_list.count-1); i++)
        {
            s_pb_mostused_contact_list.contact_table[i] = s_pb_mostused_contact_list.contact_table[i+1];
            //s_pb_mostused_contact_list.visibility_table[i] = s_pb_mostused_contact_list.visibility_table[i+1];
            //高于该优先级的记录减1
            if((s_pb_mostused_contact_list.contact_table[i].used_freq & 0xFFFF) >= prority)
            {
                s_pb_mostused_contact_list.contact_table[i].used_freq --;
            }
            
        }
        //高于该优先级的记录减1
        for (i=0; i<contact_index; i++)
        {
            if((s_pb_mostused_contact_list.contact_table[i].used_freq & 0xFFFF) >= prority)
            {
                s_pb_mostused_contact_list.contact_table[i].used_freq--;
            }
        }
        s_pb_mostused_contact_list.count--;    
        SCI_PutSemaphore(s_mostusedcontext_list_semphore_p);         
        //更新常用联系人的id使用情况表
        SetEntryIdFlag(FALSE, contact_id, &s_pb_mostused_contact_id_table);
        result = MMIPB_ERROR_SUCCESS;
    }
    return result;
}


/*****************************************************************************/
//  Description : 从常用联系人索引表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllMostUsedContactListNode(void)
{
    SCI_GetSemaphore(s_mostusedcontext_list_semphore_p, SCI_WAIT_FOREVER);
    s_pb_mostused_contact_list.count = 0;
    if(s_pb_mostused_contact_list.contact_table != PNULL)
    {
        SCI_MEMSET(s_pb_mostused_contact_list.contact_table, 0x00, sizeof(MMIPB_MOSTUSED_CONTACT_NODE_T)*MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS);
    }
    //常用联系人id使用情况表
    if(PNULL != s_pb_mostused_contact_id_table.valid_flag)
    {
        SCI_MEMSET(s_pb_mostused_contact_id_table.valid_flag,0, ((MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS + 7) / 8 ));
    }
    s_pb_mostused_contact_id_table.max_record_num = MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS;
    s_pb_mostused_contact_id_table.spare_id = 1;

    SCI_PutSemaphore(s_mostusedcontext_list_semphore_p);       
}
#endif
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 从联系人索引表中删除所以sns索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllSNSContactListNode(void)
{
    uint16 i = 0;
    uint16 sns_count = 0;
    uint16 first_sns_index = 0;
    uint16 first_char_count = 0;
    uint16 pre_first_char = 0xFFFF;
    uint16 next_first_char = 0xFFFF;
    uint16 *index_offset_ptr = PNULL;

    
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER);
    //clean sns from qsort list
    for(i =0; i < s_pb_qsort_list.count && (PNULL != s_pb_contact_list.list); i++)
    {
        if(MMIPB_IsSNSContact(ShortIDToStorageID(s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].short_storage_id)))
        {
            sns_count++;

        }
        else
        {
            if(sns_count > 0)
            {
                s_pb_qsort_list.list[i - sns_count] = s_pb_qsort_list.list[i];
            }
        }
    } 
    if(sns_count <= s_pb_qsort_list.count)
    {
           s_pb_qsort_list.count -= sns_count;
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p);
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    sns_count = 0;
    //delete all sns index node
    index_offset_ptr = (uint16 *)SCI_ALLOCA(s_pb_contact_list.list_info.count*sizeof(uint16));
    if(index_offset_ptr != PNULL)
    {
        SCI_MEMSET(index_offset_ptr, 0x00, s_pb_contact_list.list_info.count*sizeof(uint16));
    }
    for(i= 0; i < s_pb_contact_list.list_info.count && (PNULL != s_pb_contact_list.list); i++)
    {   
        if(index_offset_ptr != PNULL)
        {
            index_offset_ptr[i] = sns_count;
        }
        if(MMIPB_IsSNSContact(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
        {
            if(0 == sns_count)
            {
                //设置sns开始的索引值
                first_sns_index = i;
            }
            sns_count++; 
            
            pre_first_char = ((0 == i) ? 0xFFFF :  s_pb_contact_list.list[i -1].first_char);
            next_first_char = ((i + 1< s_pb_contact_list.list_info.count) ? s_pb_contact_list.list[i + 1].first_char:0xFFFF);
            if(s_pb_contact_list.list[i].first_char != pre_first_char
              &&s_pb_contact_list.list[i].first_char != next_first_char
              )
            {
                //这个记录的首字母和前后都不同，该记录删除后，总的首字母个数要-1
                first_char_count++;
            }
            
        }
        else
        {
            if(sns_count > 0)
            {
                //
                s_pb_contact_list.list[i - sns_count] = s_pb_contact_list.list[i];
            }
        }
    }   
    //adjust list info
    s_pb_contact_list.list_info.count = s_pb_contact_list.list_info.count - sns_count;
    if(first_char_count <= s_pb_contact_list.list_info.first_char_count)
    {
        s_pb_contact_list.list_info.first_char_count -= first_char_count;
    }
    for(i = MMIPB_GROUP_SNS_SINA; i < MMIPB_GROUP_SNS_MAX; i++)
    {
        s_pb_contact_list.list_info.group_count[i] = 0;
        s_pb_contact_list.list_info.group_mail_count[i] = 0;
        s_pb_contact_list.list_info.group_number_count[i] = 0;
    }
    s_pb_contact_list.list_info.storage_count[MMIPB_STORAGE_SNS] = 0;
    SCI_PutSemaphore(s_contact_list_semphore_p); 
    
    //adjust sqlist index
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER);     
    if(index_offset_ptr != PNULL)
    {
        for(i =0; i < s_pb_qsort_list.count && (PNULL != s_pb_contact_list.list); i++)
        {
            if(index_offset_ptr[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)] > 0)
            {
                s_pb_qsort_list.list[i] = s_pb_qsort_list.list[i] - index_offset_ptr[(s_pb_qsort_list.list[i]&MMIPB_CONTACT_INDEX_MASK)];
            }
        } 
        SCI_FREE(index_offset_ptr);
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p);
    //reset sns list
    s_pb_sns_contact_list.current_count = 0;
}
#endif
/*****************************************************************************/
//  Description : 读取对应联系人的姓名号码和email信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadContactNameNumberMail(
                                                     uint16 contact_id,
                                                     uint16 storage_id,
                                                     MMIPB_NAME_NUMBER_MAIL_T *string_ptr
                                                     )
{
    return MMIPB_GetContactNameNumberByIndex(string_ptr, MMIPB_GetContactListIndex(contact_id, storage_id));
}


/*****************************************************************************/
//  Description : 插入一个节点到快速查找表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertQListNode(
                                           uint16       contact_index, //记录索引表的索引
                                           wchar       *wchar_ptr,
                                           uint16       wchar_len,
                                           wchar       *Key_wchar_ptr,
                                           uint16       Key_wchar_len
                                           )
{
    uint32 sortlist_index = 0;
    int32 i = 0;
    int32 j = 0;
    MMI_BIN_SEARCH_INFO_T  search_info = {0};
    MMI_STRING_T           str_t[2] = {0};
    BOOLEAN             need_insert = FALSE;
    if(s_pb_qsort_list.list == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //change contact name to name letters and number letters
    if(s_pb_qsort_list.count >= s_pb_qsort_list.max_count)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER); 
    
    //adjust index
    for(i =0; i < (int32)s_pb_qsort_list.count; i++)
    {
        if((s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) >= contact_index)
        {
            s_pb_qsort_list.list[i]++;
        }
    }  
    str_t[0].wstr_ptr = wchar_ptr;
    str_t[0].wstr_len = wchar_len;
    str_t[1].wstr_ptr = Key_wchar_ptr;
    str_t[1].wstr_len = Key_wchar_len;
    for(j = 0; j < 2; j++)
    {
        //get insert postion
        search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
        search_info.start_pos = 0;
        search_info.end_pos = (int32)s_pb_qsort_list.count -1;
        if(str_t[j].wstr_len > 0 && str_t[j].wstr_ptr  != PNULL)
        {
            need_insert = TRUE;
            search_info.compare_base_data = &str_t[j];
            MMIAPICOM_BinSearch(&search_info, 
                QSortListCmp,
                &sortlist_index,
                &s_pb_qsort_list
                );
        }  
        //insert to qsort list
        if(need_insert)
        {
            if(sortlist_index < s_pb_qsort_list.count)
            {
                for(i = s_pb_qsort_list.count; i > sortlist_index ; i--) /*lint !e574 !e737*/
                {                                                        
                    s_pb_qsort_list.list[i] = s_pb_qsort_list.list[i -1];
                }  
                //SCI_MEMCPY(&s_pb_qsort_list.list[sortlist_index +1], &s_pb_qsort_list.list[sortlist_index], sizeof(uint16)*(s_pb_qsort_list.count -sortlist_index)); 
            }
            if(sortlist_index <= s_pb_qsort_list.count)
            {
                if(j == 0)
                {
                    s_pb_qsort_list.list[sortlist_index] = contact_index;
                }
                else
                {
                    s_pb_qsort_list.list[sortlist_index] = (contact_index | MMIPB_KEY_HEAD_FLAG);
                }            
            }
            s_pb_qsort_list.count++;    
            need_insert = FALSE;
        }
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p); 
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 从快速查找表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteQListNode(
                                           uint16 contact_index
                                           )
{
    uint32 i = 0;
    uint32 cur_index = 0;
    uint32 del_count = 0;
    BOOLEAN is_valid = FALSE;
    
    //找到需要删除的解决，从表中删除这些节点
    cur_index = 0;
    SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER); 
    for(i = 0; i < s_pb_qsort_list.count ; i++)
    {
        
        is_valid = TRUE;
        if((s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) > contact_index)
        {
            s_pb_qsort_list.list[i] --;
        }
        else if((s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) == contact_index)
        {
            //该数据被删除
            is_valid = FALSE;
            del_count++;
        }
        if(is_valid)
        {
            //把有效数据放到表中
            if(del_count > 0)
            {
                s_pb_qsort_list.list[cur_index] = s_pb_qsort_list.list[i]; 
            }
            cur_index++;
        }
    }
    
    if(s_pb_qsort_list.count >= del_count)
    {
        s_pb_qsort_list.count -= del_count;
    }
    SCI_PutSemaphore(s_qsort_list_semphore_p); 
    return MMIPB_ERROR_SUCCESS;    
    
}

/*****************************************************************************/
//  Description : 在快速查找表中查找
// str_input：用户的字符串
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchQSortList
(
 MMI_STRING_T *str_input,//IN
 uint32        group,
 uint32       exclude_group,
 BOOLEAN is_char//TRUE:字符查找；FALSE：数字查找
 )
{
    
    uint16 start_pos = 0;
    uint16 end_pos = 0;
    uint16 i = 0;
    //uint16 count = 0;
    uint32 contact_group = 0;
    BOOLEAN result = FALSE;
    if(PNULL != str_input && (str_input->wstr_len > 0))
    {
        if(s_pb_qsort_list.count == 0)
        {
            return 0;
        }
        //find first matched pos
        SCI_GetSemaphore(s_qsort_list_semphore_p, SCI_WAIT_FOREVER); 
        start_pos = GetQSortListIndex(str_input, is_char,FALSE);
        //find last matched pos
        end_pos = GetQSortListIndex(str_input,is_char, TRUE);
        //SCI_TRACE_LOW("[mmipb_statistics] MMIPB_SearchQSortList,start_pos=%d, end_pos=%d",start_pos,end_pos);
        //根据查找结果设置记录的可见性
        if(PB_NOT_EXIST != start_pos && (PB_NOT_EXIST != end_pos))
        {
            result = TRUE; 
            for (i = start_pos; i <= end_pos; i++)
            {                
                    //字符查找，并且类型是字符类型
                    //数字查找，并且类型是数字类型
                    //enable this entry to visible
                    contact_group = s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].group;
                    if(
                       contact_group > 0
                       &&(exclude_group & contact_group)
                      )
                    {
                        //在exclude_group中分组记录不显示
                        s_pb_contact_list.list[(s_pb_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0;
                    }
                    else if(0 == contact_group && (exclude_group & 0x01))
                    {
                        //去掉未分组
                        s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0;
                    }
                    else if((group & 0x01) || (group &contact_group))
                    {
                        //列出全部或者在所列的分组中
                        if((is_char && s_pb_qsort_list.list[i] < MMIPB_KEY_HEAD_FLAG)//字符查找，该节点是字符节点
                           || (!is_char &&s_pb_qsort_list.list[i] >= MMIPB_KEY_HEAD_FLAG)//键值查找，该节点为键值节点
                           )
                        {
                            s_pb_contact_list.list[(s_pb_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0xFF;
                        }
                    }
                }                         
        }
        SCI_PutSemaphore(s_qsort_list_semphore_p); 
    }
    else
    {
        if(s_pb_contact_list.list != PNULL)
        {
            result = TRUE;
            //enable this entry to visible
            for(i =0; i < s_pb_contact_list.list_info.count; i++)
            {
                contact_group = s_pb_contact_list.list[i].group;
                if((contact_group > 0 &&(exclude_group & contact_group))//在exclude_group分组                 
                 )
                {
                    //在exclude_group中分组记录不显示
                    s_pb_contact_list.list[i].visible_flag = 0;
                }
                else if(0 == contact_group && (exclude_group & 0x01))
                {
                    //去掉未分组
                    s_pb_contact_list.list[i].visible_flag = 0;
                }
                else if((group & 0x01) || (group &contact_group))
                {
                    //列出全部或者在所列的分组中
                    s_pb_contact_list.list[i].visible_flag = 0xFF;
                }
            }
        }
    }
    return result;   
}

/*****************************************************************************/
//  Description : 清除联系人的可见性表
// 
/*****************************************************************************/
PUBLIC void MMIPB_CleanVisibleTable(uint32 group)
{
    uint16 i = 0;
#ifdef MMIPB_MOST_USED_SUPPORT    
    if( (1<< PB_GROUP_MOSTUSED) != group)
#endif
    {
        //清除一般联系人的可见性表
        for(i = 0; i < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL; i++)
        {
            s_pb_contact_list.list[i].visible_flag = 0;
        }       
    }
}

/*****************************************************************************/
//  Description : 是所有的联系人可见
// 
/*****************************************************************************/
PUBLIC void MMIPB_EnableAllContactVisible(void)
{
    uint16 i = 0;

    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for(i = 0; i < s_pb_contact_list.list_info.count && s_pb_contact_list.list != PNULL; i++)
    {
        s_pb_contact_list.list[i].visible_flag = 0xFF;
    } 
    SCI_PutSemaphore(s_contact_list_semphore_p);
}
#ifdef MMIPB_SEARCH_CONTACT_CONTENT 
/*****************************************************************************/
//  Discription:TF  find string first not equal position
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 wchar* Wstrstr(const wchar* pString1, const wchar* pString2)
{
	wchar *pPos = (wchar *) pString1; 
	wchar *pStr2 = (wchar *) pString2; 
    uint16 string2_len = 0;

	string2_len = MMIAPICOM_Wstrlen(pString2);
	if(0 == string2_len)
	{
		return((wchar *)pString1);
	}
    MMIAPICOM_Wstrlower(pString1);
	while (*pPos)
	{
		if(0 == MMIAPICOM_Wstrncmp(pPos, pStr2, string2_len))
		{
			return(pPos);
		}
		pPos++; 
	}

	return (PNULL);
}
/*****************************************************************************/
//  Description :check two number is partial matched
//  Global resource dependence : none
//  Author: maryxiao
//Return: 
/*****************************************************************************/
LOCAL BOOLEAN SearchSubStrInContactContent(wchar* substr, MMIPB_CONTACT_T contact_info)
{
    BOOLEAN  ret = FALSE;

    do
    {
#ifdef MMIPB_MAIL_SUPPORT
        if(PNULL!=Wstrstr(contact_info.mail.wstr ,substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifdef CMCC_VCARD_SUPPORT
        if(PNULL!=Wstrstr(contact_info.alias.wstr,substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
        if(PNULL!=Wstrstr(contact_info.imessager.wstr, substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifdef SNS_SUPPORT 
        if(PNULL!=Wstrstr(contact_info.sign.wstr, substr))
        {
            ret = TRUE;
            break;
        }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
        if(PNULL!=Wstrstr(contact_info.birth.wstr, substr))
        {
            ret = TRUE;
            break;
        }
        if(PNULL!=Wstrstr(contact_info.url.wstr, substr))
        {
            ret = TRUE;
            break;
        }  
        if(PNULL!=Wstrstr(contact_info.memo.wstr, substr))
        {
            ret = TRUE;
            break;
        } 
#endif
        {
            uint8           group_id[PB_GROUP_MAX] = {0};
            uint8           group_num = 0;
            MMIPB_GROUP_T   group_info = {0};
            int             i = 0;

            group_num = MMIPB_GetAllGroupIDs(contact_info.union_t.group, group_id, PB_GROUP_MAX-1);

            for(i = 0; i < group_num; i++)
            {
                MMIPB_ReadGroup(group_id[i], &group_info);

                if(PNULL != Wstrstr(group_info.group_name.wstr, substr))
                {
                    ret = TRUE;
                    break;
                } 
            }
        }
    } while(0);

    return ret;
}
#endif
/*****************************************************************************/
//  Description : 查找名字包含该字符串的记录
//  search_str: searching string
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInList(wchar *search_str, 
                                          MMIPB_SEARCH_TYPE_T search_type,
                                          BOOLEAN                      is_char,
                                          uint32 group,
                                          uint32 exclude_group)
{
    uint32  i = 0;
    uint32  j = 0;
    uint32  contact_group = 0;
    uint16  count = 0;
    //uint16                num_len = 0;
    BOOLEAN               is_exist = FALSE;
    BOOLEAN               is_num_valid = FALSE;
    MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
    MMIPB_BCD_NUMBER_T bcd_number = {0};
    MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
    


    if(PNULL != search_str)
    {
        is_num_valid = MMIAPICOM_ConvertWStr2Num(search_str, MMIAPICOM_Wstrlen(search_str), &bcd_number);
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    if( (1<<PB_GROUP_MOSTUSED) == group)
    {
       count =  MMIPB_GetAllMostUsedContactCount();
    }
    else
#endif
    {
        SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
        //非常用联系人查找
        for(i = 0; i< s_pb_contact_list.list_info.count; i++)
        {
            is_exist = FALSE;
            if(s_pb_contact_list.list[i].visible_flag)
            {
                count++;
            }
            else
            {
                contact_group = s_pb_contact_list.list[i].group;
                if(contact_group > 0 && (exclude_group & contact_group))
                {
                    //在exclude_group中的记录不显示
                    continue;
                }
                else if(0 == contact_group && (exclude_group & 0x01))
                {
                    //去掉未分组
                    continue;
                }
                if( ((1<< PB_GROUP_ALL) & group) || (group & contact_group))
                {
                    SCI_MEMSET(&cmp_str, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));//wang.qinggang add for 490859
                    if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactNameNumberByIndex(&cmp_str, i))
                    {
                        //search
                        if(MMIPB_SEARCH_NAME & search_type)/*lint !e655*/
                        {
                            //search name
                            letters_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
                            if(letters_ptr != PNULL)
                            {
                                if(search_str != PNULL)
                                {
                                    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                                    if(MMIAPICOM_WstrnstrExt(cmp_str.name.wstr, cmp_str.name.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                    {
                                        s_pb_contact_list.list[i].visible_flag = 0xFF;
                                        is_exist = TRUE;
                                        count++;                                        
                                    }
                                    else
                                    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                                    {
                                        SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
                                        #ifdef MMI_PB_SURNAME_SUPPORT
                                        MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
                                        #else /*MMI_PB_SURNAME_SUPPORT*/
                                        MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
                                        #endif /*MMI_PB_SURNAME_SUPPORT*/

                                        #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                                        if(MMIAPICOM_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                        {
                                            s_pb_contact_list.list[i].visible_flag = 0xFF;
                                            is_exist = TRUE;
                                            count++;                                                
                                        }
                                        #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                                        if(is_char)
                                        {
                                            if(MMIAPIPB_MultiLang_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                            {
                                                s_pb_contact_list.list[i].visible_flag = 0xFF;
                                                is_exist = TRUE;
                                                count++; 
                                            }
                                        }
                                        else
                                        {
                                            if(MMIAPICOM_Wstrnstr(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str)))
                                            {
                                                s_pb_contact_list.list[i].visible_flag = 0xFF;
                                                is_exist = TRUE;
                                                count++;  
                                            }
                                        }
                                        #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                                        
                                    }
                                }
                                SCI_FREE(letters_ptr);
                                if(is_exist)
                                {
                                    //count++;
                                    continue;
                                }
                            }
                        }
                        if(MMIPB_SEARCH_NUMBER & search_type)/*lint !e655*/
                        {
                            //search number
                            for ( j = 0; j < MMIPB_MAX_NV_PHONE_NUM; j++ )
                            {   
                                // 从记录中获得号码
                                if(is_num_valid && bcd_number.number_len > 0)
                                {                                   
                                    if(MMIAPICOM_bcdsstr(&cmp_str.number_t[j], &bcd_number))
                                    {
                                        s_pb_contact_list.list[i].visible_flag |= MMIPB_FEILD_MASK_SINGLE_NUMBER << j; 
                                        count++;                                        
                                    }
                                }
                            }
                        }
#ifdef MMIPB_MAIL_SUPPORT
                        if(MMIPB_SEARCH_EMAIL & search_type)/*lint !e655*/
                        {
                            //search email
                            #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                            if(search_str != PNULL && cmp_str.mail.wstr_len >0 && MMIAPICOM_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                            #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                            if(search_str != PNULL && cmp_str.mail.wstr_len >0 && MMIAPIPB_MultiLang_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                            #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                            {
                                s_pb_contact_list.list[i].visible_flag |= MMIPB_FEILD_FLAG_MASK_MAIL;
                                count++;
                                //continue;
                            }
                        }                        
#endif
                    }
#ifdef MMIPB_SEARCH_CONTACT_CONTENT 
                if((MMIPB_SEARCH_OTHER & search_type) &&(s_pb_contact_list.list[i].visible_flag == 0))
                {
                    uint16 entry_id = 0;
                    uint16 storage_id = 0;
                    MMIPB_CONTACT_T contact;
                    storage_id = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                    
#ifdef MMIPB_MOST_USED_SUPPORT
                    if(MMIPB_IsMostUsedContact(storage_id))
                    {
                        MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node = {0};
                        mostused_contact_node = MMIPB_GetMostUsedContactBaseInfo(i);
                        entry_id = mostused_contact_node.entry_id;
                        storage_id =mostused_contact_node.storage_id;

                    }
                    else
#endif
                    {

                        MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
                        contact_node = MMIPB_GetContactBaseInfo(i);
                        entry_id = contact_node.contact_id;
                        storage_id =contact_node.storage_id;
                    }
                    SCI_MEMSET(&contact, 0X0, sizeof(MMIPB_CONTACT_T));
                    MMIPB_GetContactInfo(entry_id, storage_id, &contact);
                    if(SearchSubStrInContactContent(search_str, contact))
                    {
                            s_pb_contact_list.list[i].visible_flag = 0xFF;
                            is_exist = TRUE;
                            count++;                             
                    }

                }
#endif
                }
            }
        }
        SCI_PutSemaphore(s_contact_list_semphore_p);
    }
    return count;
}

/*****************************************************************************/
//  Description : 查找名字包含该字符串的记录
//  search_str: searching string
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInList2
(
 wchar *search_str, 
 MMIPB_SEARCH_TYPE_T search_type,
 BOOLEAN is_char,
 uint16  max_count
 )
{
	uint32  i = 0;
	uint32  j = 0;
	uint16  count = 0;
	BOOLEAN               is_exist = FALSE;
	BOOLEAN num_or_email_find = FALSE;
	BOOLEAN               is_num_valid = FALSE;
	MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
	MMIPB_BCD_NUMBER_T bcd_number = {0};
	MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
	
	if(PNULL != search_str)
	{
		is_num_valid = MMIAPICOM_ConvertWStr2Num(search_str, MMIAPICOM_Wstrlen(search_str), &bcd_number);
	}
	SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
	for(i = 0; i< s_pb_contact_list.list_info.count && count < max_count; i++)
	{
		is_exist = FALSE;
		num_or_email_find = FALSE;
		if(s_pb_contact_list.list[i].visible_flag)
		{
			count++;
		}
		else
		{
			SCI_MEMSET(&cmp_str, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
			if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactNameNumberByIndex(&cmp_str, i))
			{
				//search
				if(MMIPB_SEARCH_NAME & search_type)
				{
					//search name
					letters_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
					if(letters_ptr != PNULL)
					{
						if(search_str != PNULL)
						{
							#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
							if(MMIAPICOM_WstrnstrExt(cmp_str.name.wstr, cmp_str.name.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
							{
								s_pb_contact_list.list[i].visible_flag = 0xFF;
								is_exist = TRUE;
								count++;                                        
							}
							else
							#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
							{
								SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
								#ifdef MMI_PB_SURNAME_SUPPORT
								MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
								#else /*MMI_PB_SURNAME_SUPPORT*/
								MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
								#endif /*MMI_PB_SURNAME_SUPPORT*/
								
								#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
								if(MMIAPICOM_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
								{
									s_pb_contact_list.list[i].visible_flag = 0xFF;
									is_exist = TRUE;
									count++;                                                
								}
								#else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
								if(is_char)
								{
									if(MMIAPIPB_MultiLang_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
									{
										s_pb_contact_list.list[i].visible_flag = 0xFF;
										is_exist = TRUE;
										count++; 
									}
								}
								else
								{
									if(MMIAPICOM_Wstrnstr(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str)))
									{
										s_pb_contact_list.list[i].visible_flag = 0xFF;
										is_exist = TRUE;
										count++;  
									}
								}
								#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
								
							}
						}
						SCI_FREE(letters_ptr);
						if(is_exist)
						{
							//count++;
							continue;
						}
					}
				}
				
				if(MMIPB_SEARCH_NUMBER & search_type)
				{
					//search number
					for ( j = 0; j < MMIPB_MAX_NV_PHONE_NUM; j++ )
					{   
						// 从记录中获得号码
						if(is_num_valid && bcd_number.number_len > 0)
						{                                   
							if(MMIAPICOM_bcdsstr(&cmp_str.number_t[j], &bcd_number))
							{
								s_pb_contact_list.list[i].visible_flag |= MMIPB_FEILD_MASK_SINGLE_NUMBER << j; 
								num_or_email_find = TRUE;                                        
							}
						}
					}							
				}

				#ifdef MMIPB_MAIL_SUPPORT
				if(MMIPB_SEARCH_EMAIL & search_type)
				{
					//search email
					#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
					if(search_str != PNULL && cmp_str.mail.wstr_len >0 && MMIAPICOM_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
					#else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
					if(search_str != PNULL && cmp_str.mail.wstr_len >0 && MMIAPIPB_MultiLang_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
					#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
					{
					    s_pb_contact_list.list[i].visible_flag |= MMIPB_FEILD_FLAG_MASK_MAIL;
					    num_or_email_find = TRUE; 
					}
				}                        
				#endif
				if(num_or_email_find)
				{
				    count++;
				}

			}
		}
	}
	SCI_PutSemaphore(s_contact_list_semphore_p);
	return count;
}

/*****************************************************************************/
//  Description : 
//  search_str: email string
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetContactWithEmail
(
 wchar *search_str, //in
 uint16* entry_id_ptr, //out
 uint16* storage_id_ptr //out
 )
{
	uint32  i = 0;
	MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
	BOOLEAN result = FALSE;
	uint16 search_len = 0;
	
#ifdef MMIPB_MAIL_SUPPORT
	if(PNULL == search_str || PNULL == entry_id_ptr  ||PNULL == storage_id_ptr)
	{
		return result;
	}
	search_len = MMIAPICOM_Wstrlen(search_str);
	if(0 == search_len)
	{
		return result;
	}
	SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
	for(i = 0; i< s_pb_contact_list.list_info.count; i++)
	{
		SCI_MEMSET(&cmp_str, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
		if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactNameNumberByIndex(&cmp_str, i))
		{
			if(0 == cmp_str.mail.wstr_len || cmp_str.mail.wstr_len != search_len)
			{
				continue;
			}
			else
			{
				if(0 == MMIAPICOM_Wstrcmp(cmp_str.mail.wstr, search_str))
				{
					*entry_id_ptr = s_pb_contact_list.list[i].contact_id;
					*storage_id_ptr = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
					result = TRUE;
					break;
				}
			}
		}
	}
	SCI_PutSemaphore(s_contact_list_semphore_p);
#endif

	return result;
}

/*****************************************************************************/
//  Description : get a free contact
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFreeContactId(uint16  storage_id)
{
    uint16 free_id = 0;
#ifdef MMIPB_MOST_USED_SUPPORT
    if(MMIPB_IsMostUsedContact(storage_id))
    {
        free_id = s_pb_mostused_contact_id_table.spare_id;
    }
    else 
#endif
    if(MMIPB_IsPhoneContact(storage_id))
    {
        free_id = s_pb_phone_contact_id_table.spare_id; 
    }
    else if(MMIPB_STORAGE_SIM1 <= MMIPB_GET_STORAGE(storage_id))
    {
        if(PHONEBOOK_SIM_STORAGE == MMIPB_GET_CONTACT_TYPE(storage_id) && MMIPB_GetDualSysByStorageId(storage_id) < MMI_DUAL_SYS_MAX)
        {
            //adn contact
#ifndef MMI_DUALMODE_ENABLE
           free_id = (uint16)PHONEBOOK_GetFirstFreeContactId(MMIPB_GetDualSysByStorageId(storage_id), MMIPB_GET_CONTACT_TYPE(storage_id));
#endif
        }
#ifdef FDN_SUPPORT
        else if(PHONEBOOK_FDN_STORAGE == MMIPB_GET_CONTACT_TYPE(storage_id) && MMIPB_GetDualSysByStorageId(storage_id) < MMI_DUAL_SYS_MAX)
        {
            //fdn contact
            uint16  i = 0;
            MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
            
            fdn_context_ptr = MMIAPIFDN_GetFDNContext(MMIPB_GetDualSysByStorageId(storage_id)); 
            
            if(fdn_context_ptr != PNULL)
            {
            /*搜索到第一个为空的记录id*/
                for (i = 0; i < fdn_context_ptr->fdn_info.max_record_num; i++)
                {
                    //search a id which entry state is false
                    if (!fdn_context_ptr->entry_state[i])
                    {
                        break;
                    }
                }                
                free_id = i + 1;
                
            }
        }
#endif
    }
    return free_id;
}

/*****************************************************************************/
//  Description : set mempool handle
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetMempoolHandle(MPOOL_HANDLE handle)
{
    SCI_DisableIRQ();
    s_pb_mempool_handle = handle;
    SCI_RestoreIRQ();
}

#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set mempool handle
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetOnlySimMempoolHandle(MPOOL_HANDLE handle)
{
    SCI_DisableIRQ();
    s_pb_only_sim_mempool_handle = handle;
    SCI_RestoreIRQ();
}
#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 从g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录ID
//  Global resource
//  Author: 
//  Parameter: entry_id_ptr：[OUT] g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录ID
//             group_id_ptr：[OUT] g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录的分组ID
//  Note: 
//  Return: TURE: 存在没有读取扩展信息的记录；
//          FALSE: 不存在没有读取扩展信息的记录。
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNextNotInitEntryID(
    uint16 *entry_id_ptr,
    uint16 *storage_id_ptr
    )
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    //uint16 index = 0;

    if(entry_id_ptr == PNULL || storage_id_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindFirstNotInitEntryID parameter invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2487_112_2_18_2_43_23_185,(uint8*)"");
        return FALSE;
    }
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for (i = 0; i < s_pb_contact_list.list_info.count; i++)
    {   
        if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
        {
            if( s_pb_contact_list.list != PNULL
                && (!s_pb_contact_list.list[i].is_read_ext)
             )
            {
                *entry_id_ptr = s_pb_contact_list.list[i].contact_id;
                *storage_id_ptr = ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id);
                result = TRUE;
                break;
            }
        }
    }

    SCI_PutSemaphore(s_contact_list_semphore_p);
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_FindFirstNotInitEntryID result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2509_112_2_18_2_43_23_186,(uint8*)"d", result);
    
    return result;
}

/*****************************************************************************/
//  Description : 从内存中读取USIM 扩展信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalFromContactList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint32 i = 0;
    //uint32 num_index = 0;
    //uint16 index = 0;
    MMIPB_NUM_MAIL_LIST_T *mail_num_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
    uint8  num_index = 0;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    if(PNULL == contact_ptr || contact_id == 0)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_ReadUSIMAddtionalFromContactList invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2532_112_2_18_2_43_23_187,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for(i =0; i < s_pb_contact_list.list_info.count; i++)
    {
        if(s_pb_contact_list.list[i].contact_id == contact_id
           && ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id) == storage_id  
          )
        {
            
            if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
            {                
                mail_num_list = s_pb_contact_list.list[i].num_mail_list;
                if(mail_num_list != PNULL)
                {
                    if(s_pb_contact_list.list[i].is_read_ext == FALSE)
                    {                
                        break;
                    }
                    //read number
                    number_list = mail_num_list->number_list;
                    num_index = 1;
                    while(number_list != PNULL && num_index < MMIPB_MAX_NV_PHONE_NUM)
                    {
                         SCI_MEMCPY(&contact_ptr->number[num_index], &number_list->number, sizeof(MMIPB_BCD_NUMBER_T));
                         contact_ptr->num_type[num_index] = number_list->num_type;
                         number_list = number_list->next_ptr;
                         num_index++;
                    }
                    
                    while(num_index < MMIPB_MAX_NV_PHONE_NUM)
                    {
                        contact_ptr->num_type[num_index] = MMIPB_ADN_NUMBER+num_index;
                        num_index++;
                    }
#ifdef MMIPB_MAIL_SUPPORT
                    //read email
                    if(mail_num_list->mail_str.wstr_len > 0 && mail_num_list->mail_str.wstr_ptr != PNULL)
                    {
                        contact_ptr->mail.wstr_len = MIN(MMIPB_MAX_MAIL_LEN, mail_num_list->mail_str.wstr_len);
                        SCI_MEMCPY(&contact_ptr->mail.wstr, mail_num_list->mail_str.wstr_ptr, sizeof(wchar)*contact_ptr->mail.wstr_len);
                    }           
#endif
                    result = MMIPB_ERROR_SUCCESS;                                  
                }
            }
            break; 
        }
    }
    //SCI_PutSemaphore(s_contact_list_semphore_p);
    //SCI_TRACE_LOW("[baokun] ReadUSIMAddtional contact_id %d storage_id %d result %d", contact_id, storage_id, result);
    return result;
}

/*****************************************************************************/
//  Description : USIM 扩展信息保存到索引表中和号码查找表中
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveAddtionToList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_NUM_MAIL_LIST_T *mail_num_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *temp_list = PNULL;
    uint32 i = 0;
    //uint32 index = 0;
    uint8  num_index = 0;

    if(PNULL == contact_ptr || contact_id == 0)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_SaveAddtionToList invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_2589_112_2_18_2_43_24_188,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;

    }
    SCI_GetSemaphore(s_contact_list_semphore_p, SCI_WAIT_FOREVER);
    for(i =0; i < s_pb_contact_list.list_info.count; i++)
    {
        if(s_pb_contact_list.list[i].contact_id == contact_id
           && ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id) == storage_id  
          )
        {
            if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_contact_list.list[i].short_storage_id)))
            {     
                //SCI_TRACE_LOW("[mmipb] get same contact_id %d storage_id %d", contact_id, storage_id);
                mail_num_list = SCI_MPALLOC(sizeof(MMIPB_NUM_MAIL_LIST_T), s_pb_mempool_handle);
                if(mail_num_list != PNULL)
                {             
                    SCI_MEMSET(mail_num_list, 0x00, sizeof(MMIPB_NUM_MAIL_LIST_T));
                    s_pb_contact_list.list[i].is_read_ext = TRUE;
                    s_pb_contact_list.list[i].num_mail_list= mail_num_list;
                    //add number to num_list
                    for(num_index = 1; num_index < MMIPB_MAX_NV_PHONE_NUM; num_index++)
                    {
                        SCI_TRACE_LOW("xiaohua.liu MMIPB_SaveAddtionToList 1111\n");
	
                        //if(MMIPB_IsValidPhoneNumber(contact_ptr->number[num_index].number_len, 
                        //    contact_ptr->number[num_index].npi_ton))
                        // modified by Tong Yongli. According to Coverity CID 24662
                        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[num_index].npi_ton, 
                            contact_ptr->number[num_index].number_len))
                        {
                            number_list = (MMIPB_BCD_NUMBER_LIST_T *)SCI_MPALLOC(sizeof(MMIPB_BCD_NUMBER_LIST_T), s_pb_mempool_handle);
                            SCI_TRACE_LOW("xiaohua.liu MMIPB_SaveAddtionToList 2222\n");
				            if(number_list != PNULL)
                            {
                                if(temp_list != PNULL)
                                {
                                    //next node
                                    temp_list->next_ptr = number_list;
                                }
                                else                           
                                {
                                    //first node
                                    mail_num_list->number_list = number_list;
                                }
                                                                   
                                SCI_MEMSET(number_list, 0x00, sizeof(MMIPB_BCD_NUMBER_LIST_T));
                                SCI_MEMCPY(&number_list->number,&contact_ptr->number[num_index], sizeof(MMIPB_BCD_NUMBER_T));
                                number_list->num_type = contact_ptr->num_type[num_index];
                                temp_list = number_list;
                                s_pb_contact_list.list_info.number_count++;
                            }
                        }
                        
                    }
#ifdef CMCC_VCARD_SUPPORT
                    MMIPB_InsertAddtionNumberToList(contact_ptr);
#endif
#ifdef MMIPB_MAIL_SUPPORT
                        //read email
                        if(contact_ptr->mail.wstr_len > 0)
                        {
                            mail_num_list->mail_str.wstr_ptr = SCI_MPALLOC((contact_ptr->mail.wstr_len +1)*sizeof(wchar), s_pb_mempool_handle);
                            if(mail_num_list->mail_str.wstr_ptr  != PNULL)
                            {
                                SCI_MEMSET(mail_num_list->mail_str.wstr_ptr, 0x00, (contact_ptr->mail.wstr_len +1)*sizeof(wchar));
                                SCI_MEMCPY(mail_num_list->mail_str.wstr_ptr,
                                           contact_ptr->mail.wstr,
                                           contact_ptr->mail.wstr_len*sizeof(wchar));
                                mail_num_list->mail_str.wstr_len = contact_ptr->mail.wstr_len;
                                s_pb_contact_list.list_info.mail_count++;
                            }
                        }
#endif
                }
            }
            result = MMIPB_ERROR_SUCCESS;
            break;
        }
    }
    SCI_PutSemaphore(s_contact_list_semphore_p);
    return result;
}
#endif
#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsTCardContactExist(void)
{
   MMIPB_CONTACT_LIST_INFO_T list_info = {0};
   BOOLEAN ret = FALSE;

    //baokun need check
    if(MMIPB_ERROR_SUCCESS ==  MMIPB_ReadContactList(&list_info))
    {
    	   if(list_info.tcontact_count > 0)
    	   {
    	   	  ret = TRUE;
    	   }
    }	
    if(!ret && MMIPB_IsTcardFileExist())
    {
       ret = TRUE;
    }
    return ret;
}
#endif

#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
//  Description :获取相应组的第某个索引值开始的第一个entry_id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetFirstIdWithIndex(uint32       group_id, uint32* m_index,int *entry_id)
{
    BOOLEAN   ret = FALSE;
    uint32 index = 0;
    uint32 temp_group = 0;
    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex group_id=%d m_index=%d !",group_id,m_index);
    if(NULL == m_index)
    {
        return FALSE;
    }
    if( group_id >= MMIPB_MAX_GROUP_NUM)
    {
        SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex group_id=%d ",group_id);
        return FALSE;
    }
    if(PB_GROUP_ALL== group_id)//这里表示是手机上的
    {
        temp_group |=(1<<PB_GROUP_ALL);
        temp_group |=(1<<PB_GROUP_HOME);
        temp_group |=(1<<PB_GROUP_FRIEND);
        temp_group |=(1<<PB_GROUP_COLLEAGUE);
        temp_group |=(1<<PB_GROUP_SCHOOLMATE);
#ifdef MMIPB_MOST_USED_SUPPORT 
        temp_group |=(1<<PB_GROUP_MOSTUSED);
        temp_group |=(1<<PB_GROUP_FAVROUTE);
#endif
    }
    else
    {
        temp_group = (1<<group_id);
    }
    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex count=%d,temp_group=0x%x ",s_pb_contact_list.list_info.count,temp_group);
	for(index = *m_index; index < s_pb_contact_list.list_info.count; index++)
	{
	    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex group=%d ",s_pb_contact_list.list[index].group);

        if(PB_GROUP_ALL==s_pb_contact_list.list[index].group && PB_GROUP_ALL==group_id)
        {
            break;
        }
        if(temp_group&s_pb_contact_list.list[index].group)
        {
            break;
        }
        
	}
	if(index<s_pb_contact_list.list_info.count)
	{
        if(NULL != entry_id)
        {
            *entry_id = s_pb_contact_list.list[index].contact_id;
            SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex  index=%d,entry_id=%d",index,*entry_id);
        }
        *m_index = index;
        ret = TRUE;
	}
    SCI_TRACE_LOW("[MCARE] MMIPB_GetFirstIdWithIndex ret=%d ",ret);
	return ret;
}

#endif//MCARE_V31_SUPPORT

#ifdef MMIPB_SYNC_WITH_O365
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  RETURN: MMIPB_ERROR_SUCCESS:
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactOffice365IdByIndex
(
 uint16* contact_id,//out
 uint16* storage_id,//out
 char*  o365_id,//out
 uint16        pos     //IN: 在存储器中的位置
 )
{
	MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
	
	if(PNULL == contact_id || PNULL == storage_id || PNULL == o365_id)
	{
		return MMIPB_ERROR_INVALID_PARAM;
	}  
	
	if(s_pb_contact_list.list != PNULL && pos < s_pb_contact_list.list_info.count)
	{
		*contact_id = s_pb_contact_list.list[pos].contact_id;
		*storage_id = ShortIDToStorageID(s_pb_contact_list.list[pos].short_storage_id);
		SCI_MEMCPY(o365_id, s_pb_contact_list.list[pos].o365_id, sizeof(s_pb_contact_list.list[pos].o365_id));
		result = MMIPB_ERROR_SUCCESS;
	}
	return (result);
}
#endif /*MMIPB_SYNC_WITH_O365*/

#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QOnlySimSortListCmp(uint32 base_index, void *compare_data, void *list)
{
  MMIPB_QSORT_LIST_T  *qsort_list = (MMIPB_QSORT_LIST_T * )list;
  //MMIPB_NAME_T         name     = {0};
  MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
  MMI_STRING_T       *input_str = (MMI_STRING_T *)compare_data;
  MMIPB_NAME_NUMBER_MAIL_T *cmp_str_ptr = PNULL;
  int ret = 0;
  BOOLEAN  is_char = TRUE;
  
  letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
  if(PNULL == letters_ptr)
  {
      return -1;
  }
  if(PNULL == qsort_list || PNULL == qsort_list->list)
  {
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_568_112_2_18_2_43_19_176,(uint8*)"");

      SCI_FREE(letters_ptr);
      return -1;
  }
  cmp_str_ptr = (MMIPB_NAME_NUMBER_MAIL_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_NUMBER_MAIL_T));
  if(cmp_str_ptr != PNULL)
  {
      SCI_MEMSET(cmp_str_ptr, 0x00, sizeof(MMIPB_NAME_NUMBER_MAIL_T));
      MMIPB_GetOnlySimContactNameNumberByIndex(cmp_str_ptr, (qsort_list->list[base_index] &MMIPB_CONTACT_INDEX_MASK));  
      //get name_in_list_t 第range_index个character对应的PinYin sn
      SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
      if(qsort_list->list[base_index] >= MMIPB_KEY_HEAD_FLAG)
      {
          //key value
          is_char = FALSE;
      }
      else
      {
          is_char = TRUE;
      }
#ifdef MMI_PB_SURNAME_SUPPORT
      MMIPB_GetNameLettersWithSurname(&cmp_str_ptr->name, &cmp_str_ptr->surname, MMIPB_NAME_HEAD_CHAR, is_char, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
      MMIPB_GetNameLetters(&cmp_str_ptr->name, MMIPB_NAME_HEAD_CHAR, is_char, letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
      ret = MMIAPICOM_Wstrcmp(letters_ptr->head_letters, input_str->wstr_ptr);
      SCI_FREE(cmp_str_ptr);
  }
  
  SCI_FREE(letters_ptr);
  return ret;
}

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int QOnlySimSortListNCmp(uint32 base_index, void *compare_data, void *list)
{
  MMIPB_QSORT_LIST_T  *qsort_list = (MMIPB_QSORT_LIST_T * )list;
  //MMIPB_NAME_T         name     = {0};
  MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
  MMIPB_QSORT_CMP_DATA_T  *cmp_data_ptr = (MMIPB_QSORT_CMP_DATA_T *)compare_data;
  //wchar               *name_str = PNULL;
  MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
  int ret = 0;

  if(cmp_data_ptr == PNULL || cmp_data_ptr->str_ptr == PNULL)
  {
        return -1;
  }
  letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
  if(PNULL == letters_ptr)
  {
      return -1;
  }
  if(PNULL == qsort_list || PNULL == qsort_list->list)
  {
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_615_112_2_18_2_43_19_177,(uint8*)"");
      SCI_FREE(letters_ptr);
      return -1;
  }
  MMIPB_GetOnlySimContactNameNumberByIndex(&cmp_str, (qsort_list->list[base_index] & MMIPB_CONTACT_INDEX_MASK));  
  //get name_in_list_t 第range_index个character对应的PinYin sn
  SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
  if(qsort_list->list[base_index] >= MMIPB_KEY_HEAD_FLAG)
  {
#ifdef MMI_PB_SURNAME_SUPPORT
      MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_HEAD_CHAR, FALSE, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
      MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_HEAD_CHAR, TRUE, letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
  }
  else
  {       
#ifdef MMI_PB_SURNAME_SUPPORT
      MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_HEAD_CHAR, TRUE, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
      MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_HEAD_CHAR, TRUE, letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
  }
  ret = MMIAPICOM_Wstrncmp(letters_ptr->head_letters, cmp_data_ptr->str_ptr->wstr_ptr, cmp_data_ptr->str_ptr->wstr_len);
  
  SCI_FREE(letters_ptr);
  return ret;
}

/*****************************************************************************/
//  Description :根据字符串在快速查找表中查找对应的index
//str_input:需要查找的字符串
//is_back: TRUE:查找最后一个匹配的位置;FALSE:查找第一个匹配的位置
// 返回匹配的位置
/*****************************************************************************/
LOCAL uint16 GetOnlySimQSortListIndex
  (
  MMI_STRING_T *str_input,//IN
  BOOLEAN      is_char,//IN
  BOOLEAN      is_back
  )
{
  MMI_BIN_SEARCH_INFO_T search_info = {0};
  uint32                  ret_pos = 0;
  BIN_COMPARE_FUNC        func = PNULL;
  MMIPB_QSORT_CMP_DATA_T cmp_data = {0};
  
  if((PNULL!= s_pb_only_sim_qsort_list.list) 
      && (s_pb_only_sim_qsort_list.count > 0))
  {
      //use binary search
      if(is_back)
      {
          //查找最后一个相等的记录
          search_info.search_type = MMICOM_SEARCH_LAST_EQUAL;
      }
      else
      {
          //查找第一个相等的记录
          search_info.search_type = MMICOM_SEARCH_FIRST_EQUAL;
      }

      cmp_data.is_char = is_char;
      cmp_data.str_ptr = str_input;
      search_info.start_pos = 0;
      search_info.end_pos = s_pb_only_sim_qsort_list.count-1;
      search_info.compare_base_data = &cmp_data;
      //find matched string in sort list table
      func = QOnlySimSortListNCmp;
      if(MMIAPICOM_BinSearch
          (&search_info, 
          (BIN_COMPARE_FUNC)func,
          &ret_pos,
          &s_pb_only_sim_qsort_list
          ))
      {
          ret_pos = PB_NOT_EXIST;
      }
  }
  return (uint16)ret_pos;
}

/*****************************************************************************/
//  Description : compare list_ptr with compare_data
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int OnlySimContactListCmp(uint32 base_index, void *compare_data, void *list)
{
    MMIPB_CONTACT_LIST_NODE_T  *contact_list = (MMIPB_CONTACT_LIST_NODE_T * )list;
    //MMIPB_NAME_T         name     = {0};
    MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;
    MMIPB_NAME_LETTERS_T  *input_str = (MMIPB_NAME_LETTERS_T *)compare_data;
    MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
    int                     ret = 0;
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    wchar*                   input_full_letter_ptr = PNULL;
    wchar*                   cmp_full_letter_ptr =PNULL;
    uint16                  pos_index = 0;
#endif
    //uint16 name_len = 0;
    //uint16 i = 0;
    if(PNULL == contact_list)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_DATALIST_1255_112_2_18_2_43_21_179,(uint8*)"");
        return -1;
    }
    letters_ptr = (MMIPB_NAME_LETTERS_T *)SCI_ALLOC_APPZ(sizeof(MMIPB_NAME_LETTERS_T));
    if(PNULL == letters_ptr)
    {
        return -1;
    }
    
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    input_full_letter_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_NAME_MAX_LEN*6+1));
    if(PNULL == input_full_letter_ptr)
    {
        SCI_FREE(letters_ptr);
        return -1;
    }
    cmp_full_letter_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIPB_NAME_MAX_LEN*6+1));
    if(PNULL == cmp_full_letter_ptr)
    {
        SCI_FREE(letters_ptr);
        SCI_FREE(input_full_letter_ptr);
    }
#endif    
    MMIPB_GetOnlySimContactNameNumberByIndex(&cmp_str, base_index);

#ifdef MMI_PB_SURNAME_SUPPORT
    MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_FULL_CHAR/*MMIPB_NAME_FULL_CHAR_SPACER*/, TRUE,letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
    MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR_SPACER, TRUE,letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/

#ifdef MMIPB_COVERT_LETTER_SUPPORT
    for(pos_index=0;pos_index<input_str->full_letters_len; pos_index++)
    {
        input_full_letter_ptr[pos_index] = ConvertLetters(input_str->full_letters[pos_index]);
    }
    for(pos_index=0;pos_index<letters_ptr->full_letters_len; pos_index++)
    {
        cmp_full_letter_ptr[pos_index] = ConvertLetters(letters_ptr->full_letters[pos_index]);  
    }

    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
    ret = MMIAPICOM_Wstrcmp(cmp_full_letter_ptr, input_full_letter_ptr);
    #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    ret = MMIPB_MultiLang_Wstrcmp(cmp_full_letter_ptr, input_full_letter_ptr);
    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    if(ret == 0)
    {
       #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
       ret = MMIAPICOM_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
       #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
       ret = MMIPB_MultiLang_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
       #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    }
#else
    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
    ret = MMIAPICOM_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
    #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
    ret = MMIPB_MultiLang_Wstrcmp(letters_ptr->full_letters, input_str->full_letters);
    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
#endif

#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
    if(ret < 0 
      && !MMIAPIIM_IsPunct(*letters_ptr->full_letters) 
      &&MMIAPIIM_IsPunct(*input_str->full_letters) 
      )
    {
        //符号不能排在非符号的后面
        ret = 1;
    }
    else if(ret > 0 
      && MMIAPIIM_IsPunct(*letters_ptr->full_letters) 
      &&!MMIAPIIM_IsPunct(*input_str->full_letters) 
      )
    {
        //符号不能排在非符号的后面
        ret = -1;
    }
#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

    SCI_FREE(letters_ptr);
#ifdef MMIPB_COVERT_LETTER_SUPPORT
    SCI_FREE(cmp_full_letter_ptr);
    SCI_FREE(input_full_letter_ptr);
#endif
    return ret;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitOnlySimDataList(void)
{
	s_pb_only_sim_contact_list.list_info.count = 0;
	SCI_MEMSET(&s_pb_only_sim_contact_list.list_info, 0x00, sizeof(MMIPB_CONTACT_LIST_INFO_T));
	s_pb_only_sim_qsort_list.count = 0;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_AllocOnlySimcontactList(void)
{
	
	uint32  item_count = 0;
	item_count = (MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX);
	SCI_TRACE_LOW("[MMIPB] MMIPB_AllocOnlySimcontactList 1");
	if(!s_pb_only_sim_mem_alloc)
	{
		MMIPB_InitOnlySimDataList();
		//快速查找表的初始化和内存分配
		s_pb_only_sim_qsort_list.list = (uint16 *)SCI_ALLOCA(item_count *2*sizeof(uint16));
		if(s_pb_only_sim_qsort_list.list == PNULL)
		{
			return FALSE;
		}
		
		SCI_MEMSET(s_pb_only_sim_qsort_list.list, 0x00, item_count*2 *sizeof(uint16));
		s_pb_only_sim_qsort_list.max_count = item_count*2;
		
		
		//重新分配索引表 	
		s_pb_only_sim_contact_list.list = (MMIPB_CONTACT_LIST_NODE_T *)SCI_ALLOCA(item_count *sizeof(MMIPB_CONTACT_LIST_NODE_T));
		if(s_pb_only_sim_contact_list.list == PNULL)
		{
			SCI_FREE(s_pb_only_sim_qsort_list.list);
			return FALSE;
		}
		
		SCI_MEMSET(s_pb_only_sim_contact_list.list, 0x00, item_count*sizeof(MMIPB_CONTACT_LIST_NODE_T));
		s_pb_only_sim_contact_list.max_count = item_count;
		SCI_MEMSET(&s_pb_only_sim_contact_list.list_info, 0x00, sizeof(MMIPB_CONTACT_LIST_INFO_T));
		s_pb_only_sim_mem_alloc = TRUE;
	}
	SCI_TRACE_LOW("[MMIPB] MMIPB_AllocOnlySimcontactList 2");
	return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_FreeOnlySimcontactList(void)
{
	
	uint32  item_count = 0;
	item_count = (MMIPB_SIM_MAX_RECORD_NUM*MMI_DUAL_SYS_MAX);
	SCI_TRACE_LOW("[MMIPB] MMIPB_FreeOnlySimcontactList 1");
	SCI_GetSemaphore(s_qsort_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
	if(s_pb_only_sim_qsort_list.list != PNULL)
	{
		SCI_FREE(s_pb_only_sim_qsort_list.list);
		s_pb_only_sim_qsort_list.list = PNULL;
		SCI_TRACE_LOW("[mmipb] MMIPB_FreeOnlySimcontactList s_pb_only_sim_qsort_list success");
	}
	SCI_PutSemaphore(s_qsort_only_sim_list_semphore_p); 
	
	SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
	if(s_pb_only_sim_contact_list.list != PNULL)
	{
#ifdef CMCC_VCARD_SUPPORT
		uint16 index = 0;
		for(index = 0; index < s_pb_only_sim_contact_list.list_info.count; index++)
		{
			MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
			MMIPB_BCD_NUMBER_LIST_T *free_list = PNULL;
			if(s_pb_only_sim_contact_list.list[index].num_mail_list != PNULL)
			{
				number_list = s_pb_only_sim_contact_list.list[index].num_mail_list->number_list;
				while(number_list != PNULL)
				{
					free_list = number_list;
					number_list = number_list->next_ptr;
					SCI_MPFREE(free_list);
					free_list = PNULL;
				}
				if(s_pb_only_sim_contact_list.list[index].num_mail_list->mail_str.wstr_ptr != PNULL)
				{
					SCI_MPFREE(s_pb_only_sim_contact_list.list[index].num_mail_list->mail_str.wstr_ptr);
				}
				SCI_MPFREE(s_pb_only_sim_contact_list.list[index].num_mail_list);
				s_pb_only_sim_contact_list.list[index].num_mail_list = PNULL;
			}
		}
#endif 
		SCI_FREE(s_pb_only_sim_contact_list.list);
		s_pb_only_sim_contact_list.list = PNULL;
		SCI_TRACE_LOW("[mmipb] MMIPB_FreeOnlySimcontactList s_pb_only_sim_contact_list success");
	}
	SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);

	MMIPB_InitOnlySimDataList();
	s_pb_only_sim_mem_alloc = FALSE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateOnlySimDataListSemphore(void)
{
	s_contact_only_sim_list_semphore_p = SCI_CreateSemaphore("s_contact_only_sim_list_semphore_p", 1);
	s_qsort_only_sim_list_semphore_p = SCI_CreateSemaphore("s_qsort_only_sim_list_semphore_p", 1);    
}

/*****************************************************************************/
//  Description : 插入一个节点到快速查找表
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertOnlySimQListNode(
                                           uint16       contact_index, //记录索引表的索引
                                           wchar       *wchar_ptr,
                                           uint16       wchar_len,
                                           wchar       *Key_wchar_ptr,
                                           uint16       Key_wchar_len
                                           )
{
	uint32 sortlist_index = 0;
	int32 i = 0;
	int32 j = 0;
	MMI_BIN_SEARCH_INFO_T  search_info = {0};
	MMI_STRING_T           str_t[2] = {0};
	BOOLEAN             need_insert = FALSE;
	if(s_pb_only_sim_qsort_list.list == PNULL)
	{
		return MMIPB_ERROR_INVALID_PARAM;
	}
	//change contact name to name letters and number letters
	if(s_pb_only_sim_qsort_list.count >= s_pb_only_sim_qsort_list.max_count)
	{
		return MMIPB_ERROR_ERROR;
	}
	SCI_GetSemaphore(s_qsort_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
	
	//adjust index
	for(i =0; i < (int32)s_pb_only_sim_qsort_list.count; i++)
	{
		if((s_pb_only_sim_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) >= contact_index)
		{
			s_pb_only_sim_qsort_list.list[i]++;
		}
	}  
	str_t[0].wstr_ptr = wchar_ptr;
	str_t[0].wstr_len = wchar_len;
	str_t[1].wstr_ptr = Key_wchar_ptr;
	str_t[1].wstr_len = Key_wchar_len;
	for(j = 0; j < 2; j++)
	{
		//get insert postion
		search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
		search_info.start_pos = 0;
		search_info.end_pos = (int32)s_pb_only_sim_qsort_list.count -1;
		if(str_t[j].wstr_len > 0 && str_t[j].wstr_ptr  != PNULL)
		{
			need_insert = TRUE;
			search_info.compare_base_data = &str_t[j];
			MMIAPICOM_BinSearch(&search_info, 
				QOnlySimSortListCmp,
				&sortlist_index,
				&s_pb_only_sim_qsort_list
				);
		}  
		//insert to qsort list
		if(need_insert)
		{
			if(sortlist_index < s_pb_only_sim_qsort_list.count)
			{
				for(i = s_pb_only_sim_qsort_list.count; i > sortlist_index ; i--) /*lint !e574 !e737*/
				{                                                        
					s_pb_only_sim_qsort_list.list[i] = s_pb_only_sim_qsort_list.list[i -1];
				}  
				//SCI_MEMCPY(&s_pb_only_sim_qsort_list.list[sortlist_index +1], &s_pb_only_sim_qsort_list.list[sortlist_index], sizeof(uint16)*(s_pb_only_sim_qsort_list.count -sortlist_index)); 
			}
			if(sortlist_index <= s_pb_only_sim_qsort_list.count)
			{
				if(j == 0)
				{
					s_pb_only_sim_qsort_list.list[sortlist_index] = contact_index;
				}
				else
				{
					s_pb_only_sim_qsort_list.list[sortlist_index] = (contact_index | MMIPB_KEY_HEAD_FLAG);
				}            
			}
			s_pb_only_sim_qsort_list.count++;    
			need_insert = FALSE;
		}
	}
	SCI_PutSemaphore(s_qsort_only_sim_list_semphore_p); 
	return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : 插入一个节点到记录表
//  Global 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertOnlySimContactListNode(
                                                 MMIPB_CONTACT_T *contact_ptr//[IN]
                                                 )
{
	MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
	MMI_BIN_SEARCH_INFO_T search_info = {0};
	uint16                count = MMIPB_INVALID_INDEX;
	//uint16                first_wchar = 0;
	uint32                pos = 0;
	uint32                i = 0;
	MMIPB_NAME_LETTERS_T  *letters_ptr = PNULL;
	MMIPB_CONTACT_BASE_INFO_T *simple_info_ptr = PNULL;
	
	wchar *key_wstr_ptr = PNULL;

	if(contact_ptr == PNULL)
	{
		return MMIPB_ERROR_INVALID_PARAM;
	}
	if(s_pb_only_sim_contact_list.list == PNULL)
	{
		return MMIPB_ERROR_INVALID_PARAM;
	}
	if(s_pb_only_sim_contact_list.list_info.count >= s_pb_only_sim_contact_list.max_count)
	{
		return result;
	}
	letters_ptr = (MMIPB_NAME_LETTERS_T  *)SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
	if(PNULL == letters_ptr)
	{
		return MMIPB_ERROR_ERROR;
	}
	SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
	SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
#ifdef MMI_PB_SURNAME_SUPPORT
	MMIPB_GetNameLettersWithSurname(&contact_ptr->name, &contact_ptr->surname, MMIPB_NAME_FULL_CHAR/*MMIPB_NAME_FULL_CHAR_SPACER*/|MMIPB_NAME_HEAD_CHAR,TRUE,letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
	MMIPB_GetNameLetters(&contact_ptr->name, MMIPB_NAME_FULL_CHAR_SPACER|MMIPB_NAME_HEAD_CHAR,TRUE,letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/
	search_info.search_type = MMICOM_SEARCH_ANY_EQUAL;
	search_info.compare_base_data = letters_ptr;
	search_info.start_pos = 0;
	
	//find insert postion in s_pb_only_sim_contact_list
	if(s_pb_only_sim_contact_list.list_info.count > 0 && s_pb_only_sim_contact_list.list_info.count < s_pb_only_sim_contact_list.max_count)
	{
		search_info.end_pos = s_pb_only_sim_contact_list.list_info.count -1;
		MMIAPICOM_BinSearch(&search_info,
			OnlySimContactListCmp,
			&pos,
			s_pb_only_sim_contact_list.list);
	}
	//insert node 
	if(s_pb_only_sim_contact_list.list_info.count < s_pb_only_sim_contact_list.max_count && pos < s_pb_only_sim_contact_list.max_count)
	{
		//SCI_DisableIRQ();
		// 顺序后移
		for(i=s_pb_only_sim_contact_list.list_info.count; i>pos; i--)
		{
			s_pb_only_sim_contact_list.list[i] = s_pb_only_sim_contact_list.list[i-1];
		}   
		//clear old data
		SCI_MEMSET(&s_pb_only_sim_contact_list.list[pos], 0x00, sizeof(MMIPB_CONTACT_LIST_NODE_T));
		//group
		s_pb_only_sim_contact_list.list[pos].group = contact_ptr->union_t.group;
		//contact id
		s_pb_only_sim_contact_list.list[pos].contact_id = contact_ptr->contact_id;
		//set index 
		s_pb_only_sim_contact_list.list[pos].index = count;
		//set start char
		#ifndef MMI_PB_MULTILANG_SORT_SUPPORT
		if(MMIAPIIM_IsPunct(letters_ptr->full_letters[0]))
		{
			s_pb_only_sim_contact_list.list[pos].first_char = '#';  
		}
		else
		#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
		{
		    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
		        s_pb_only_sim_contact_list.list[pos].first_char = letters_ptr->full_letters[0];
		    #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
		        s_pb_only_sim_contact_list.list[pos].first_char = MMIPB_MultiLang_GetFirstChar(letters_ptr->full_letters[0]);
		    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
		}
		if(pos == 0 || s_pb_only_sim_contact_list.list[pos].first_char != s_pb_only_sim_contact_list.list[pos -1].first_char)
		{
			//第一个记录，或者和上一个记录的首字母不相同
			s_pb_only_sim_contact_list.list_info.first_char_count++;
		}
		//storage id
		s_pb_only_sim_contact_list.list[pos].short_storage_id = StorageIDToShortID(contact_ptr->storage_id);
#ifdef MMIPB_UID_SUPPORT        
		s_pb_only_sim_contact_list.list[pos].uid = contact_ptr->uid;
#endif
#ifdef CMCC_VCARD_SUPPORT
		if(MMIPB_IsUsim(contact_ptr->storage_id))
		{
			s_pb_only_sim_contact_list.list[pos].is_read_ext = FALSE;
		}
		else
		{
			s_pb_only_sim_contact_list.list[pos].is_read_ext = TRUE;
		}
#endif
		s_pb_contact_list.list[pos].photo_id_flag = contact_ptr->photo_id_flag;
		s_pb_only_sim_contact_list.list_info.count++;
		simple_info_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_BASE_INFO_T));
		if(PNULL != simple_info_ptr)
		{
			SCI_MEMSET(simple_info_ptr, 0x00, sizeof(MMIPB_CONTACT_BASE_INFO_T));
			MMIPB_ReadOnlySimContactSimpleInfo(pos, simple_info_ptr);
			//adjust base info
			result = AdjustContactListInfo(simple_info_ptr,
				TRUE,
				&s_pb_only_sim_contact_list.list_info);
			SCI_FREE(simple_info_ptr);

		}                
		
		if(letters_ptr->head_letters_len > 0)
		{
			#ifndef MMI_PB_SURNAME_SUPPORT
			key_wstr_ptr = SCI_ALLOCA((MMIPB_NAME_MAX_LEN +1)*sizeof(wchar));
			#else
			key_wstr_ptr = SCI_ALLOCA(((MMIPB_NAME_MAX_LEN +1)*2)*sizeof(wchar));
			#endif

			if(key_wstr_ptr != PNULL)
			{
				SCI_MEMSET(key_wstr_ptr, 0x00, (MMIPB_NAME_MAX_LEN +1)*sizeof(wchar));
				MMIIM_GetWcharKeyValueByTable(letters_ptr->head_letters, key_wstr_ptr, FALSE);
			}
		}
		MMIPB_InsertOnlySimQListNode(pos, 
			letters_ptr->head_letters, 
			letters_ptr->head_letters_len,
			key_wstr_ptr,
			letters_ptr->head_letters_len);
		if(key_wstr_ptr != PNULL)
		{
			SCI_FREE(key_wstr_ptr);
		}
		SCI_FREE(letters_ptr);
	}
	SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
	
	return result;
}

/*****************************************************************************/
//  Description : 查找名字包含该字符串的记录
//  search_str: searching string
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC uint16 MMIPB_SearchSubStringInOnlySimList(wchar *search_str, 
                                          MMIPB_SEARCH_TYPE_T search_type,
                                          BOOLEAN                      is_char,
                                          uint32 group,
                                          uint32 exclude_group)
{
	uint32  i = 0;
	uint32  j = 0;
	uint32  contact_group = 0;
	uint16  count = 0;
	BOOLEAN               is_exist = FALSE;
	BOOLEAN               is_num_valid = FALSE;
	MMIPB_NAME_NUMBER_MAIL_T cmp_str = {0};
	MMIPB_BCD_NUMBER_T bcd_number = {0};
	MMIPB_NAME_LETTERS_T *letters_ptr = PNULL;

	if(s_pb_only_sim_contact_list.list == PNULL)
	{
		return 0;
	}

	if(PNULL != search_str)
	{
		is_num_valid = MMIAPICOM_ConvertWStr2Num(search_str, MMIAPICOM_Wstrlen(search_str), &bcd_number);
	}
	SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER);
	//非常用联系人查找
	for(i = 0; i< s_pb_only_sim_contact_list.list_info.count; i++)
	{
            is_exist = FALSE;
            if(s_pb_only_sim_contact_list.list[i].visible_flag)
            {
                count++;
            }
            else
            {
                contact_group = s_pb_only_sim_contact_list.list[i].group;
                if(contact_group > 0 && (exclude_group & contact_group))
                {
                    //在exclude_group中的记录不显示
                    continue;
                }
                else if(0 == contact_group && (exclude_group & 0x01))
                {
                    //去掉未分组
                    continue;
                }
                if( ((1<< PB_GROUP_ALL) & group) || (group & contact_group))
                {
                    if(MMIPB_ERROR_SUCCESS == MMIPB_GetOnlySimContactNameNumberByIndex(&cmp_str, i))
                    {
                        //search
                        if(MMIPB_SEARCH_NAME & search_type)/*lint !e655*/
                        {
                            //search name
                            letters_ptr = SCI_ALLOCA(sizeof(MMIPB_NAME_LETTERS_T));
                            if(letters_ptr != PNULL)
                            {
                                if(search_str != PNULL)
                                {
                                    #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                                    if(MMIAPICOM_WstrnstrExt(cmp_str.name.wstr, cmp_str.name.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                    #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                                    if(MMIAPIPB_MultiLang_WstrnstrExt(cmp_str.name.wstr, cmp_str.name.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                    #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                                    {
                                        s_pb_only_sim_contact_list.list[i].visible_flag = 0xFF;
                                        is_exist = TRUE;
                                        count++;                                        
                                    }
                                    else
                                    {
                                        SCI_MEMSET(letters_ptr, 0x00, sizeof(MMIPB_NAME_LETTERS_T));
		
#ifdef MMI_PB_SURNAME_SUPPORT
                                        MMIPB_GetNameLettersWithSurname(&cmp_str.name, &cmp_str.surname, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
#else /*MMI_PB_SURNAME_SUPPORT*/
                                        MMIPB_GetNameLetters(&cmp_str.name, MMIPB_NAME_FULL_CHAR , is_char, letters_ptr);
#endif /*MMI_PB_SURNAME_SUPPORT*/

                                        #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                                        if(MMIAPICOM_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                        #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                                        if(MMIAPIPB_MultiLang_WstrnstrExt(letters_ptr->full_letters, letters_ptr->full_letters_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                                        #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

                                        {
                                            s_pb_only_sim_contact_list.list[i].visible_flag = 0xFF;
                                            is_exist = TRUE;
                                            count++;                                                
                                        }
                                    }
                                }
                                SCI_FREE(letters_ptr);
                                if(is_exist)
                                {
                                    //count++;
                                    continue;
                                }
                            }
                        }
                        if(MMIPB_SEARCH_NUMBER & search_type)/*lint !e655*/
                        {
                            //search number
                            for ( j = 0; j < MMIPB_MAX_NV_PHONE_NUM; j++ )
                            {   
                                // 从记录中获得号码
                                if(is_num_valid && bcd_number.number_len > 0)
                                {                                   
                                    if(MMIAPICOM_bcdsstr(&cmp_str.number_t[j], &bcd_number))
                                    {
                                        s_pb_only_sim_contact_list.list[i].visible_flag |= MMIPB_FEILD_MASK_SINGLE_NUMBER << j; 
                                        count++;                                        
                                    }
                                }
                            }
                        }
#ifdef MMIPB_MAIL_SUPPORT
                        if(MMIPB_SEARCH_EMAIL & search_type)/*lint !e655*/
                        {
                            //search email
                            #ifndef MMI_PB_MULTILANG_SORT_SUPPORT
                            if(search_str != PNULL && MMIAPICOM_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                            #else /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                            if(search_str != PNULL && MMIAPIPB_MultiLang_WstrnstrExt(cmp_str.mail.wstr, cmp_str.mail.wstr_len, search_str, MMIAPICOM_Wstrlen(search_str), FALSE))
                            #endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/
                            {
                                s_pb_only_sim_contact_list.list[i].visible_flag |= MMIPB_FEILD_FLAG_MASK_MAIL;
                                count++;
                                //continue;
                            }
                        }                        
#endif
                    }
#ifdef MMIPB_SEARCH_CONTACT_CONTENT 
                if((MMIPB_SEARCH_OTHER & search_type) &&(s_pb_only_sim_contact_list.list[i].visible_flag == 0))
                {
                    uint16 entry_id = 0;
                    uint16 storage_id = 0;
                    MMIPB_CONTACT_T contact;
                    MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
                    storage_id = ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id);
                    contact_node = MMIPB_GetOnlySimContactBaseInfo(i);
                    entry_id = contact_node.contact_id;
                    storage_id =contact_node.storage_id;
                    SCI_MEMSET(&contact, 0X0, sizeof(MMIPB_CONTACT_T));
                    MMIPB_GetContactInfo(entry_id, storage_id, &contact);
                    if(SearchSubStrInContactContent(search_str, contact))
                    {
                            s_pb_only_sim_contact_list.list[i].visible_flag = 0xFF;
                            is_exist = TRUE;
                            count++;                             
                    }
                }
#endif
                }
            }
	}
	SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
	return count;
}

/*****************************************************************************/
//  Description : 读记录索引表的基本信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadOnlySimContactList(
                                           MMIPB_CONTACT_LIST_INFO_T *data_ptr//[OUT]
                                           )
{
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    *data_ptr = s_pb_only_sim_contact_list.list_info;
    //SCI_RestoreIRQ();
    
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : Find visiable contact from start_index
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindOnlySimContact(
                                       uint16 start_index,  
                                       uint32 group_mask,//[IN]所有要查找的群组
                                       uint16 storage_id, //要查找的联系人类型                                        
                                       uint16 *find_index,//[OUT]
                                       MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                       MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT]
                                       )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 i = 0;
    uint16 j = 0;
    
    //get a visibile contact from start_index
    if(valid_index_array != PNULL)
    {
        //初始化有效号码索引和email数组
        SCI_MEMSET(valid_index_array->visible_num_index, 0x0, sizeof(valid_index_array->visible_num_index));
        valid_index_array->is_mail_visible = FALSE;
    }
    if(MMIPB_IsSIMContact(storage_id))
    {
        SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER);
        if(s_pb_only_sim_contact_list.list != PNULL)
        {
            for(i = start_index; i < s_pb_only_sim_contact_list.list_info.count; i++)
            {
                if(( ((1<< PB_GROUP_ALL) &group_mask) || group_mask & s_pb_only_sim_contact_list.list[i].group)//全部分组或包含要检索的分组
                    && s_pb_only_sim_contact_list.list[i].visible_flag > 0//可见
                    )
                {
                    //如果要检索的分组是全部分组或该记录分组包含要检索的分组，并且该记录可见
                    result = MMIPB_ERROR_SUCCESS;
                    find_data_ptr->contact_id = s_pb_only_sim_contact_list.list[i].contact_id;
                    find_data_ptr->first_char = s_pb_only_sim_contact_list.list[i].first_char;
                    find_data_ptr->photo_id_flag = s_pb_only_sim_contact_list.list[i].photo_id_flag;
                    find_data_ptr->group = s_pb_only_sim_contact_list.list[i].group;
                    find_data_ptr->index = i;
                    find_data_ptr->mail_exist = IsContactHasMail(&(s_pb_only_sim_contact_list.list[i]));
                    find_data_ptr->storage_id = ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id);
                    GetContactNumType(&(s_pb_only_sim_contact_list.list[i]), find_data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
                    if(find_index != PNULL)
                    {
                        //set find index
                        *find_index = i; 
                    }
                    //get visible number index
                    for(j =0; j < MMIPB_MAX_NV_PHONE_NUM; j++)
                    {
                        
                        if(s_pb_only_sim_contact_list.list[i].visible_flag < (1 << j))
                        {
                            break;
                        }
                        if(valid_index_array != PNULL && find_data_ptr->num_type[j] == MMIPB_INVLID_NUMBER)
                        {
//                            if(valid_index_array != PNULL)
                            {
                                valid_index_array->visible_num_index[j] = 0;
                            }
                        }
                        else if(s_pb_only_sim_contact_list.list[i].visible_flag & (1 << j))
                        {
                            //该号码可见; this number is visible
                            if(valid_index_array != PNULL)
                            {
                                valid_index_array->visible_num_index[j] = 1;
                            }
                        }
                        
                    }
                    //set  visible 
                    if(valid_index_array != PNULL)
                    {
                        //该mail可见；this mail is visible
                        if(IsContactHasMail(&(s_pb_only_sim_contact_list.list[i])) && (s_pb_only_sim_contact_list.list[i].visible_flag &MMIPB_FEILD_FLAG_MASK_MAIL))
                        {
                            valid_index_array->is_mail_visible = TRUE;
                        }
                    }
                    break;
                }
            }
        }
        //SCI_RestoreIRQ();
        SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
    }
#if defined (MMIPB_SIMDN_SUPPORT) || defined(FDN_SUPPORT)    
    else
    {
        //other contact list
        result = FindDailNumberContact(start_index, storage_id, find_index, valid_index_array, find_data_ptr);
    }
#endif    
    return result;
}

/*****************************************************************************/
//  Description : 获取该联系人的简单信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadOnlySimContactSimpleInfo(
                                               uint16 index, 
                                               MMIPB_CONTACT_BASE_INFO_T *data_ptr//[OUT]
                                               )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    
    if(data_ptr == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_DisableIRQ();
    if(index < s_pb_only_sim_contact_list.list_info.count && s_pb_only_sim_contact_list.list != PNULL)
    {
        data_ptr->group = s_pb_only_sim_contact_list.list[index].group;//GetContactGroup(s_pb_only_sim_contact_list.list[index]);
        data_ptr->contact_id = s_pb_only_sim_contact_list.list[index].contact_id;
        data_ptr->first_char = s_pb_only_sim_contact_list.list[index].first_char;
        data_ptr->index = s_pb_only_sim_contact_list.list[index].index;
        data_ptr->storage_id = ShortIDToStorageID(s_pb_only_sim_contact_list.list[index].short_storage_id);
        data_ptr->visible_flag = s_pb_only_sim_contact_list.list[index].visible_flag;
        data_ptr->photo_id_flag = s_pb_only_sim_contact_list.list[index].photo_id_flag;
        GetContactNumType(&(s_pb_only_sim_contact_list.list[index]), data_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
        data_ptr->mail_exist = IsContactHasMail(&(s_pb_only_sim_contact_list.list[index]));
        result = MMIPB_ERROR_SUCCESS;
    }
    //SCI_RestoreIRQ();
    
    return result;
}

/*****************************************************************************/
//  Description : 从快速查找表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOnlySimQListNode(
                                           uint16 contact_index
                                           )
{
    uint32 i = 0;
    uint32 cur_index = 0;
    uint32 del_count = 0;
    BOOLEAN is_valid = FALSE;

    if(s_pb_only_sim_qsort_list.list == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
	
    //找到需要删除的解决，从表中删除这些节点
    cur_index = 0;
    SCI_GetSemaphore(s_qsort_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
    for(i = 0; i < s_pb_only_sim_qsort_list.count ; i++)
    {
        
        is_valid = TRUE;
        if((s_pb_only_sim_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) > contact_index)
        {
            s_pb_only_sim_qsort_list.list[i] --;
        }
        else if((s_pb_only_sim_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK) == contact_index)
        {
            //该数据被删除
            is_valid = FALSE;
            del_count++;
        }
        if(is_valid)
        {
            //把有效数据放到表中
            if(del_count > 0)
            {
                s_pb_only_sim_qsort_list.list[cur_index] = s_pb_only_sim_qsort_list.list[i]; 
            }
            cur_index++;
        }
    }
    
    if(s_pb_only_sim_qsort_list.count >= del_count)
    {
        s_pb_only_sim_qsort_list.count -= del_count;
    }
    SCI_PutSemaphore(s_qsort_only_sim_list_semphore_p); 
    return MMIPB_ERROR_SUCCESS;    
    
}

/*****************************************************************************/
//  Description : 从记录表中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOnlySimContactListNode(
                                                 uint16 contact_id,
                                                 uint16 storage_id
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 i = 0;
    //uint32 dest_index = 0;
    //uint32 del_count = 0;
    uint16 index = 0;
    uint16 cache_index = 0;
    //uint16 storage_id = 0;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPB_CONTACT_BASE_INFO_T simple_info = {0};
    index = MMIPB_GetOnlySimContactListIndex(contact_id, storage_id);
   
    if(PNULL == s_pb_only_sim_contact_list.list)
    {
    	result = MMIPB_ERROR_SUCCESS;
    }
    if(index < s_pb_only_sim_contact_list.list_info.count && s_pb_only_sim_contact_list.list != PNULL)
    {
        result = MMIPB_ERROR_SUCCESS;
        SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
        MMIPB_ReadOnlySimContactSimpleInfo(index, &simple_info);
        cache_index = s_pb_only_sim_contact_list.list[index].index;
        //从缓存表中删除


#ifdef CMCC_VCARD_SUPPORT
          if(MMIPB_IsUsim(storage_id))
          {
              MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
              MMIPB_BCD_NUMBER_LIST_T *free_list = PNULL;
              if(s_pb_only_sim_contact_list.list[index].num_mail_list != PNULL)
              {
                  number_list = s_pb_only_sim_contact_list.list[index].num_mail_list->number_list;
                  while(number_list != PNULL)
                  {
                      free_list = number_list;
                      number_list = number_list->next_ptr;
                      SCI_MPFREE(free_list);
                      free_list = PNULL;
                  }
                  if(s_pb_only_sim_contact_list.list[index].num_mail_list->mail_str.wstr_ptr != PNULL)
                  {
                      SCI_MPFREE(s_pb_only_sim_contact_list.list[index].num_mail_list->mail_str.wstr_ptr);
                  }
                  SCI_MPFREE(s_pb_only_sim_contact_list.list[index].num_mail_list);
                  s_pb_only_sim_contact_list.list[index].num_mail_list = PNULL;

              }
          }
#endif 
        //从索引表中
        for(i = 0; i < s_pb_only_sim_contact_list.list_info.count; i++)
        {
            if(i >= index && i < s_pb_only_sim_contact_list.list_info.count -1)
            {
               s_pb_only_sim_contact_list.list[i] = s_pb_only_sim_contact_list.list[i + 1];
            }


            if(
               storage_id == ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id)
               && s_pb_only_sim_contact_list.list[i].index > cache_index
              )

            {
                s_pb_only_sim_contact_list.list[i].index--;
            }

        }
        
        //该节点将被删除，调整基本信息
        AdjustContactListInfo(&simple_info, FALSE, &s_pb_only_sim_contact_list.list_info);            
        s_pb_only_sim_contact_list.list_info.count--;
        
        SCI_PutSemaphore(s_contact_only_sim_list_semphore_p); 
        //从快速查找表中删除
        //delete index from qsort list
        MMIPB_DeleteOnlySimQListNode(index);
    }
    return result;
}

/*****************************************************************************/
//  Description : Delete one group's contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteOneGroupOnlySimContactList(uint32 group)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 i = 0;
    uint16 del_count = 0;
    MMIPB_CONTACT_BASE_INFO_T contact_info = {0};

    if(s_pb_only_sim_qsort_list.list == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }

    if(group > 0)
    {
        //delete qsort list
        for(i = 0; i < s_pb_only_sim_qsort_list.count; i++)
        {
            if(group & s_pb_only_sim_contact_list.list[(s_pb_only_sim_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].group)
            {
                //该节点需要删除
                del_count++;                
            }
            else
            {
                if(del_count > 0)
                {
                    s_pb_only_sim_qsort_list.list[i - del_count] = s_pb_only_sim_qsort_list.list[i];
                }
            }
        }
        if(del_count <= s_pb_only_sim_qsort_list.count)
        {
            s_pb_only_sim_qsort_list.count = s_pb_only_sim_qsort_list.count - del_count;
        }

        //delete contact list
        del_count = 0;
        for(i = 0; i < s_pb_only_sim_contact_list.list_info.count; i++)
        {
            if(group & s_pb_only_sim_contact_list.list[i].group)
            {
                //该节点需要删除
                if(MMIPB_ERROR_SUCCESS == MMIPB_ReadOnlySimContactSimpleInfo(i, &contact_info))
                {
                    AdjustContactListInfo(&contact_info, FALSE, &s_pb_only_sim_contact_list.list_info);                        
                    SCI_MEMSET(&contact_info, 0x00, sizeof(contact_info));
                }
                del_count++;                
            }
            else
            {
                if(del_count > 0)
                {
                    s_pb_only_sim_contact_list.list[i - del_count] = s_pb_only_sim_contact_list.list[i];
                }
            }
        }
        if(del_count <= s_pb_only_sim_contact_list.list_info.count)
        {
            s_pb_only_sim_contact_list.list_info.count = s_pb_only_sim_contact_list.list_info.count - del_count;
        }        
        result = MMIPB_ERROR_SUCCESS;
    }    
    return result;
}

/*****************************************************************************/
//  Description : 根据记录id和storage id来确定该记录在索引表的索引
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetOnlySimContactListIndex(
                                        uint16 contact_id,
                                        uint16 stoage_id
                                        )
{
    
    uint16 index = MMIPB_INVALID_INDEX;
    uint16 i = 0;
    
    //SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER); ;
    if(contact_id > 0 && s_pb_only_sim_contact_list.list != PNULL)
    {
        for(i = 0; i < s_pb_only_sim_contact_list.list_info.count; i++)
        {
            if(contact_id == s_pb_only_sim_contact_list.list[i].contact_id
                && stoage_id == ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id)
                )
            {
                index = i;
                break;
            }
        }
        
    }
    //SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
    
    return index;
}

/*****************************************************************************/
//  Description : 在快速查找表中查找
// str_input：用户的字符串
// group：要查找的记录所在的group;
// exclude_group:需要排除的group数组，如果要查找的记录在这个group数组中，不显示 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SearchOnlySimQSortList
(
 MMI_STRING_T *str_input,//IN
 uint32        group,
 uint32       exclude_group,
 BOOLEAN is_char//TRUE:字符查找；FALSE：数字查找
 )
{
    
    uint16 start_pos = 0;
    uint16 end_pos = 0;
    uint16 i = 0;
    //uint16 count = 0;
    uint32 contact_group = 0;
    BOOLEAN result = FALSE;
    if(PNULL != str_input && (str_input->wstr_len > 0))
    {
        if(s_pb_only_sim_qsort_list.count == 0)
        {
            return 0;
        }
        //find first matched pos
        SCI_GetSemaphore(s_qsort_only_sim_list_semphore_p, SCI_WAIT_FOREVER); 
        start_pos = GetOnlySimQSortListIndex(str_input, is_char,FALSE);
        //find last matched pos
        end_pos = GetOnlySimQSortListIndex(str_input,is_char, TRUE);
        //根据查找结果设置记录的可见性
        if(PB_NOT_EXIST != start_pos && (PB_NOT_EXIST != end_pos))
        {
            result = TRUE; 
            for (i = start_pos; i <= end_pos; i++)
            {                
                    //字符查找，并且类型是字符类型
                    //数字查找，并且类型是数字类型
                    //enable this entry to visible
                    contact_group = s_pb_only_sim_contact_list.list[(s_pb_only_sim_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].group;
                    if(
                       contact_group > 0
                       &&(exclude_group & contact_group)
                      )
                    {
                        //在exclude_group中分组记录不显示
                        s_pb_only_sim_contact_list.list[(s_pb_only_sim_qsort_list.list[i] & MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0;
                    }
                    else if(0 == contact_group && (exclude_group & 0x01))
                    {
                        //去掉未分组
                        s_pb_only_sim_contact_list.list[(s_pb_only_sim_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0;
                    }
                    else if((group & 0x01) || (group &contact_group))
                    {
                        //列出全部或者在所列的分组中
                        if((is_char && s_pb_only_sim_qsort_list.list[i] < MMIPB_KEY_HEAD_FLAG)//字符查找，该节点是字符节点
                           || (!is_char &&s_pb_only_sim_qsort_list.list[i] >= MMIPB_KEY_HEAD_FLAG)//键值查找，该节点为键值节点
                           )
                        {
                            s_pb_only_sim_contact_list.list[(s_pb_only_sim_qsort_list.list[i] &MMIPB_CONTACT_INDEX_MASK)].visible_flag = 0xFF;
                        }
                    }
                }                         
        }
        SCI_PutSemaphore(s_qsort_only_sim_list_semphore_p); 
    }
    else
    {
        if(s_pb_only_sim_contact_list.list != PNULL)
        {
            result = TRUE;
            //enable this entry to visible
            for(i =0; i < s_pb_only_sim_contact_list.list_info.count; i++)
            {
                contact_group = s_pb_only_sim_contact_list.list[i].group;
                if((contact_group > 0 &&(exclude_group & contact_group))//在exclude_group分组                 
                 )
                {
                    //在exclude_group中分组记录不显示
                    s_pb_only_sim_contact_list.list[i].visible_flag = 0;
                }
                else if(0 == contact_group && (exclude_group & 0x01))
                {
                    //去掉未分组
                    s_pb_only_sim_contact_list.list[i].visible_flag = 0;
                }
                else if((group & 0x01) || (group &contact_group))
                {
                    //列出全部或者在所列的分组中
                    s_pb_only_sim_contact_list.list[i].visible_flag = 0xFF;
                }
            }
        }
    }
    return result;   
}

/*****************************************************************************/
//  Description : 清除联系人的可见性表
// 
/*****************************************************************************/
PUBLIC void MMIPB_CleanOnlySimVisibleTable(uint32 group)
{
    uint16 i = 0;
        //清除一般联系人的可见性表
        for(i = 0; i < s_pb_only_sim_contact_list.list_info.count && s_pb_only_sim_contact_list.list != PNULL; i++)
        {
            s_pb_only_sim_contact_list.list[i].visible_flag = 0;
        }       
}

/*****************************************************************************/
//  Description : 是所有的联系人可见
// 
/*****************************************************************************/
PUBLIC void MMIPB_EnableOnlySimAllContactVisible(void)
{
    uint16 i = 0;

    SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER);
    for(i = 0; i < s_pb_only_sim_contact_list.list_info.count && s_pb_only_sim_contact_list.list != PNULL; i++)
    {
        s_pb_only_sim_contact_list.list[i].visible_flag = 0xFF;
    } 
    SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
}

/*****************************************************************************/
//  Description : 从联系人索引表中读取联系人姓名
//  Global resource dependence : none
//  Author: baokun.yin
//  RETURN: MMIPB_ERROR_SUCCESS:读成功; 返回其他值 :操作失败
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetOnlySimContactNameNumberByIndex(
                                                 MMIPB_NAME_NUMBER_MAIL_T *cmp_str,//[OUT]: 比较字符串
                                                 uint16        pos     //IN: 在存储器中的位置
                                                 )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_ENTRY;
    uint16 storage_id = 0;
    uint16 cache_index = 0;
    MMI_STRING_T name_str = {0};
#ifdef MMI_PB_SURNAME_SUPPORT
    MMI_STRING_T surname_str = {0};
#endif /*MMI_PB_SURNAME_SUPPORT*/
#ifdef MMIPB_MAIL_SUPPORT
    MMI_STRING_T mail_str = {0};
#endif
    MMIPB_CONTACT_T *contact_ptr = PNULL; 
    MMIPB_ACCESS_POINT_T access_point = {0};
   
    if((PNULL == cmp_str))
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }  
    
    if(s_pb_only_sim_contact_list.list != PNULL && pos < s_pb_only_sim_contact_list.list_info.count)
    {
        storage_id = ShortIDToStorageID(s_pb_only_sim_contact_list.list[pos].short_storage_id);

        cache_index = s_pb_only_sim_contact_list.list[pos].index;

        if((MMIPB_EMPTY_STRING_FLAG != name_str.wstr_len && name_str.wstr_ptr == PNULL)           
#ifdef MMIPB_MAIL_SUPPORT
            &&(MMIPB_EMPTY_STRING_FLAG != mail_str.wstr_len && mail_str.wstr_ptr == PNULL)
#endif
            )
        {
            contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(contact_ptr != PNULL)
            {
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                access_point.contact.contact_id = s_pb_only_sim_contact_list.list[pos].contact_id;
                access_point.contact.storage_id = storage_id;
                result = MMIPB_Read(MMIPB_ACCESS_CONTACT,
                    access_point,
                    sizeof(MMIPB_CONTACT_T),
                    contact_ptr
                    );
                if(MMIPB_ERROR_SUCCESS == result)
                {
                    SCI_MEMCPY(&cmp_str->name, &contact_ptr->name, sizeof(cmp_str->name));
#ifdef MMI_PB_SURNAME_SUPPORT
                    SCI_MEMCPY(&cmp_str->surname, &contact_ptr->surname, sizeof(cmp_str->surname));
#endif /*MMI_PB_SURNAME_SUPPORT*/
                    SCI_MEMCPY(&cmp_str->number_t, contact_ptr->number, sizeof(cmp_str->number_t));
#ifdef MMIPB_MAIL_SUPPORT
                    SCI_MEMCPY(&cmp_str->mail, &contact_ptr->mail, sizeof(cmp_str->mail));
#endif
                    cmp_str->group = contact_ptr->union_t.group;
                     result = MMIPB_ERROR_SUCCESS;
                }
                SCI_FREE(contact_ptr);
            }
           
        }
        else
        {
            if(name_str.wstr_len > 0 && name_str.wstr_ptr != PNULL)
            {
                cmp_str->name.wstr_len = MIN(name_str.wstr_len, MMIPB_NAME_MAX_LEN);
                SCI_MEMCPY(&cmp_str->name.wstr, name_str.wstr_ptr, sizeof(wchar)*cmp_str->name.wstr_len);               
            }

#ifdef MMI_PB_SURNAME_SUPPORT			
            if(surname_str.wstr_len > 0 && surname_str.wstr_ptr != PNULL)
            {
                cmp_str->surname.wstr_len = MIN(surname_str.wstr_len, MMIPB_NAME_MAX_LEN);
                SCI_MEMCPY(&cmp_str->surname.wstr, surname_str.wstr_ptr, sizeof(wchar)*cmp_str->surname.wstr_len);               
            }
#endif /*MMI_PB_SURNAME_SUPPORT*/

#ifdef MMIPB_MAIL_SUPPORT
            if(mail_str.wstr_len > 0 && mail_str.wstr_ptr != PNULL)
            {
                cmp_str->mail.wstr_len = MIN(mail_str.wstr_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMCPY(&cmp_str->mail.wstr, mail_str.wstr_ptr, sizeof(wchar)*cmp_str->mail.wstr_len);               
            }
#endif
            result = MMIPB_ERROR_SUCCESS;
        }
    }
    return (result);
}

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 从g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录ID
//  Global resource
//  Author: 
//  Parameter: entry_id_ptr：[OUT] g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录ID
//             group_id_ptr：[OUT] g_mmipb_entry_list中查找第一个还没有读取扩展信息的记录的分组ID
//  Note: 
//  Return: TURE: 存在没有读取扩展信息的记录；
//          FALSE: 不存在没有读取扩展信息的记录。
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetOnlySimNextNotInitEntryID(
    uint16 *entry_id_ptr,
    uint16 *storage_id_ptr
    )
{
    BOOLEAN result = FALSE;
    uint32  i = 0;
    //uint16 index = 0;

    if(entry_id_ptr == PNULL || storage_id_ptr == PNULL)
    {
        return FALSE;
    }
    SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER);
    for (i = 0; i < s_pb_only_sim_contact_list.list_info.count; i++)
    {   
        if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id)))
        {
            if( s_pb_only_sim_contact_list.list != PNULL
                && (!s_pb_only_sim_contact_list.list[i].is_read_ext)
             )
            {
                *entry_id_ptr = s_pb_only_sim_contact_list.list[i].contact_id;
                *storage_id_ptr = ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id);
                result = TRUE;
                break;
            }
        }
    }

    SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
    
    return result;
}

/*****************************************************************************/
//  Description : 从内存中读取USIM 扩展信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalFromOnlySimContactList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint32 i = 0;
    //uint32 num_index = 0;
    //uint16 index = 0;
    MMIPB_NUM_MAIL_LIST_T *mail_num_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
    uint8  num_index = 0;
    //MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    if(PNULL == contact_ptr || contact_id == 0)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    if(s_pb_only_sim_contact_list.list == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    //SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER);
    for(i =0; i < s_pb_only_sim_contact_list.list_info.count; i++)
    {
        if(s_pb_only_sim_contact_list.list[i].contact_id == contact_id
           && ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id) == storage_id  
          )
        {
            
            if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id)))
            {                
                mail_num_list = s_pb_only_sim_contact_list.list[i].num_mail_list;
                if(mail_num_list != PNULL)
                {
                    if(s_pb_only_sim_contact_list.list[i].is_read_ext == FALSE)
                    {                
                        break;
                    }
                    //read number
                    number_list = mail_num_list->number_list;
                    num_index = 1;
                    while(number_list != PNULL && num_index < MMIPB_MAX_NV_PHONE_NUM)
                    {
                         SCI_MEMCPY(&contact_ptr->number[num_index], &number_list->number, sizeof(MMIPB_BCD_NUMBER_T));
                         contact_ptr->num_type[num_index] = number_list->num_type;
                         number_list = number_list->next_ptr;
                         num_index++;
                    }
                    
                    while(num_index < MMIPB_MAX_NV_PHONE_NUM)
                    {
                        contact_ptr->num_type[num_index] = MMIPB_ADN_NUMBER+num_index;
                        num_index++;
                    }
#ifdef MMIPB_MAIL_SUPPORT
                    //read email
                    if(mail_num_list->mail_str.wstr_len > 0 && mail_num_list->mail_str.wstr_ptr != PNULL)
                    {
                        contact_ptr->mail.wstr_len = MIN(MMIPB_MAX_MAIL_LEN, mail_num_list->mail_str.wstr_len);
                        SCI_MEMCPY(&contact_ptr->mail.wstr, mail_num_list->mail_str.wstr_ptr, sizeof(wchar)*contact_ptr->mail.wstr_len);
                    }           
#endif
                    result = MMIPB_ERROR_SUCCESS;                                  
                }
            }
            break; 
        }
    }
    //SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
    return result;
}

/*****************************************************************************/
//  Description : USIM 扩展信息保存到索引表中和号码查找表中
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveAddtionToOnlySimList(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_NUM_MAIL_LIST_T *mail_num_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *number_list = PNULL;
    MMIPB_BCD_NUMBER_LIST_T *temp_list = PNULL;
    uint32 i = 0;
    //uint32 index = 0;
    uint8  num_index = 0;

    if(PNULL == contact_ptr || contact_id == 0)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    if(s_pb_only_sim_contact_list.list == PNULL)
    {
        return MMIPB_ERROR_INVALID_PARAM;
    }
    SCI_GetSemaphore(s_contact_only_sim_list_semphore_p, SCI_WAIT_FOREVER);
    for(i =0; i < s_pb_only_sim_contact_list.list_info.count; i++)
    {
        if(s_pb_only_sim_contact_list.list[i].contact_id == contact_id
           && ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id) == storage_id  
          )
        {
            if(MMIPB_IsUsim(ShortIDToStorageID(s_pb_only_sim_contact_list.list[i].short_storage_id)))
            {     
                mail_num_list = SCI_MPALLOC(sizeof(MMIPB_NUM_MAIL_LIST_T), s_pb_mempool_handle);
                if(mail_num_list != PNULL)
                {             
                    SCI_MEMSET(mail_num_list, 0x00, sizeof(MMIPB_NUM_MAIL_LIST_T));
                    s_pb_only_sim_contact_list.list[i].is_read_ext = TRUE;
                    s_pb_only_sim_contact_list.list[i].num_mail_list= mail_num_list;
                    //add number to num_list
                    for(num_index = 1; num_index < MMIPB_MAX_NV_PHONE_NUM; num_index++)
                    {
                        SCI_TRACE_LOW("xiaohua.liu MMIPB_SaveAddtionToOnlySimList 1111\n");
	
                        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[num_index].npi_ton, 
                            contact_ptr->number[num_index].number_len))
                        {
                            number_list = (MMIPB_BCD_NUMBER_LIST_T *)SCI_MPALLOC(sizeof(MMIPB_BCD_NUMBER_LIST_T), s_pb_only_sim_mempool_handle);
                            SCI_TRACE_LOW("xiaohua.liu MMIPB_SaveAddtionToOnlySimList 2222\n");
				            if(number_list != PNULL)
                            {
                                if(temp_list != PNULL)
                                {
                                    //next node
                                    temp_list->next_ptr = number_list;
                                }
                                else                           
                                {
                                    //first node
                                    mail_num_list->number_list = number_list;
                                }
                                                                   
                                SCI_MEMSET(number_list, 0x00, sizeof(MMIPB_BCD_NUMBER_LIST_T));
                                SCI_MEMCPY(&number_list->number,&contact_ptr->number[num_index], sizeof(MMIPB_BCD_NUMBER_T));
                                number_list->num_type = contact_ptr->num_type[num_index];
                                temp_list = number_list;
                                s_pb_only_sim_contact_list.list_info.number_count++;
                            }
                        }
                        
                    }
#ifdef CMCC_VCARD_SUPPORT
                    //MMIPB_InsertAddtionNumberToList(contact_ptr);
#endif
#ifdef MMIPB_MAIL_SUPPORT
                        //read email
                        if(contact_ptr->mail.wstr_len > 0)
                        {
                            mail_num_list->mail_str.wstr_ptr = SCI_MPALLOC((contact_ptr->mail.wstr_len +1)*sizeof(wchar), s_pb_only_sim_mempool_handle);
                            if(mail_num_list->mail_str.wstr_ptr  != PNULL)
                            {
                                SCI_MEMSET(mail_num_list->mail_str.wstr_ptr, 0x00, (contact_ptr->mail.wstr_len +1)*sizeof(wchar));
                                SCI_MEMCPY(mail_num_list->mail_str.wstr_ptr,
                                           contact_ptr->mail.wstr,
                                           contact_ptr->mail.wstr_len*sizeof(wchar));
                                mail_num_list->mail_str.wstr_len = contact_ptr->mail.wstr_len;
                                s_pb_only_sim_contact_list.list_info.mail_count++;
                            }
                        }
#endif
                }
                else
                {
                    s_pb_only_sim_contact_list.list[i].is_read_ext = TRUE;
                    SCI_TRACE_LOW("[MMIPB] MMIPB_SaveAddtionToOnlySimList 555");
                }
            }
            result = MMIPB_ERROR_SUCCESS;
            break;
        }
    }
    SCI_PutSemaphore(s_contact_only_sim_list_semphore_p);
    return result;
}
#endif /*CMCC_VCARD_SUPPORT*/

#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/



