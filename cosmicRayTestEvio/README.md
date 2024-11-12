# cosmicRayTestEvio Mannuals
>
> For the common usage, please look up the `README.md` in `../cosmicRayTest/`

## Cutting condition
To switch the cutting condition on or off, please change the variable `addCuts` in `JEventProcessor_cosmicRayTestEvio.h`.
|  Cuts       |      Variables |      Explainnation      |
|   :--:      |      :--:      |        :--:             |
|    col > 20       |      `col>20`         |         The left half dector is off         |
|    nGood       |      `nGood`         |      With in an trigger, after other cuts we expected at least 5 channels are left, if not we discard the whole trigger            |
| A channel muse have some neighbors to be accepted       |      `hasNeighbor[i]`          |    To supress background. For now the restriction is not so tight, we expect another channel  lighten with in 3 rows, 1 column. If not satisfied we **only discard this channel** in this trigger              |    
|    No multiple neighbors       |      `multiNeighbor[i]`    |  We discard all the channels whose both adjacent neighbors in the same row are lightened. Just to supress background |
|           |               |                  |
|           |               |                  |
|           |               |                  |
|           |               |                  |
|           |               |                  |

## Root File gengerated
The file will include a `TTree` object, in order to conveniently combine all the data in different run. As it is hard to record 2D data in TTree, I adopt another way which is include the channel index (starts from the upper right corner of ECAL and goes down until next column). So if we want to extract the plots using the `.root` file, simply convert the channel index into the column and row, the euqation  is as follows:


\begin{equation}
\eta
\end{euqation}