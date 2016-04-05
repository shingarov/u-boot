# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

obj-y   = ast2400.o flash.o flash_spi.o pci.o crc32.o slt.o regtest.o vfun.o vhace.o crt.o videotest.o mactest.o hactest.o mictest.o

ifeq ($(CONFIG_FPGA_ASPEED),y)
obj-y   += platform_fpga.o
else ifeq ($(CONFIG_SLT_ASPEED),y)
obj-y	+= platform_slt.o
else
obj-y   += platform.o
endif
