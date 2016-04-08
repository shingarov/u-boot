/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef SWFUNC_H
#define SWFUNC_H

/* == Step 4:==========   Select PHY    ================== */
//#define SUPPORT_PHY_LAN9303        // Initial PHY via I2C bus
#define LAN9303_I2C_BUSNUM      6  // 1-based
#define LAN9303_I2C_ADR         0x14

/* =================  Check setting  ===================== */
#ifdef USE_P2A
        #error Can NOT be set PCI bus in Uboot
#endif
#ifdef USE_LPC
        #error Can NOT be set LPC bus in Uboot
#endif
#ifdef SPI_BUS
        #error Can NOT be set SPI bus in Uboot
#endif

/* ======================== Program flow control ======================== */
#define RUN_STEP                     5
// 0: read_scu
// 1: parameter setup
// 2: init_scu1,
// 3: init_scu_macrst
// 4: Data Initial
// 5: ALL

/* ====================== Switch print debug message ====================== */
#define   DbgPrn_Enable_Debug_delay  0
//#define   DbgPrn_FuncHeader          0 //1
#define   DbgPrn_ErrFlg              0
#define   DbgPrn_BufAdr              0 //1
#define   DbgPrn_Bufdat              0
#define   DbgPrn_BufdatDetail        0
#define   DbgPrn_PHYRW               0
#define   DbgPrn_PHYInit             0
#define   DbgPrn_PHYName             0
#define   DbgPrn_DumpMACCnt          0
#define   DbgPrn_Info                0 //1
#define   DbgPrn_FRAME_LEN           0


/* ============ Enable or Disable Check item of the descriptor ============ */
#define   CheckRxOwn
#define   CheckRxErr
//#define   CheckOddNibble
#define   CheckCRC
#define   CheckRxFIFOFull
#define   CheckRxLen
//#define   CheckDataEveryTime

//#define   CheckRxbufUNAVA
#define   CheckRPktLost
//#define   CheckNPTxbufUNAVA
#define   CheckTPktLost
#define   CheckRxBuf

#endif // SWFUNC_H
